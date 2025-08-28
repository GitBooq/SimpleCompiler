#include "Parser.hpp"

#include <stdexcept>

namespace parser {

// Helper: convert a character into a Tag
static constexpr lexer::Tag sym(char c) {
  return static_cast<lexer::Tag>(static_cast<unsigned char>(c));
}

// Get the next token from the lexer
void Parser::move() { look = lex->scan(); }

// Expect a token with a specific tag
void Parser::match(lexer::Tag t) {
  if (look->tag == t)
    move();
  else
    throw std::runtime_error("Syntax error: unexpected token '" +
                             look->toString() + "'");
}

// Overload to match a single-character token
void Parser::match(char ch) {
  match(static_cast<lexer::Tag>(static_cast<unsigned char>(ch)));
}

// Parse the whole program
void Parser::program() { block(); }

// Parse a block
void* Parser::block() {
  match('{');
  auto savedEnv = top;
  top = std::make_shared<symbols::Env>(top);
  decls();
  stmts();
  match('}');
  top = savedEnv;
  return nullptr;
}

// Parse variable declarations
void Parser::decls() {
  while (look->tag == lexer::Tag::BASIC) {
    auto p = type();
    auto tok = look;
    match(lexer::Tag::ID);

    auto id = std::make_shared<symbols::Id>(tok->lexeme, p, bytesUsed);
    top->put(tok->lexeme, id);
    bytesUsed += p->width;

    if (look->tag == lexer::Tag::ASSIGN) {
      move();
      auto initExpr = assign();  // save init expr if needed
    }
    match(';');
  }
}

// Parse a type
sptr<symbols::Type> Parser::type() {
  auto tt = std::dynamic_pointer_cast<lexer::TypeToken>(look);
  if (!tt) throw std::runtime_error("Expected type, got: " + look->toString());
  match(lexer::Tag::BASIC);
  auto p = tt->typeInfo;
  if (look->tag == sym('[')) p = dims(p);
  return p;
}

// Parse array dimensions recursively
sptr<symbols::Type> Parser::dims(sptr<symbols::Type> p) {
  match('[');
  int size = std::stoi(look->lexeme);
  match(lexer::Tag::NUM);
  match(']');
  if (look->tag == sym('[')) p = dims(p);
  return std::make_shared<symbols::Array>(size, p);
}

// Parse multiple statements
void Parser::stmts() {
  while (look->tag != sym('}') && look->tag != lexer::Tag::END) stmt();
}

// Parse a single statement
void Parser::stmt() {
  if (look->tag == sym('{')) {
    block();
  } else if (look->tag == lexer::Tag::ID) {
    auto e = assign();
    match(';');
  } else {
    throw std::runtime_error("Unknown statement start: " + look->toString());
  }
}

// Assignment expression
sptr<ast::Expr> Parser::assign() {
  auto left = orExpr();
  if (look->tag == lexer::Tag::ASSIGN) {
    auto tok = look;
    auto loc = tok->loc;
    move();
    auto right = assign();
    return std::make_shared<ast::Op>(loc, tok, left, right);
  }
  return left;
}

// Logical OR
sptr<ast::Expr> Parser::orExpr() {
  auto expr = andExpr();
  while (look->tag == lexer::Tag::OR) {
    auto tok = look;
    auto loc = tok->loc;
    move();
    expr = std::make_shared<ast::Or>(loc, tok, expr, andExpr());
  }
  return expr;
}

// Logical AND
sptr<ast::Expr> Parser::andExpr() {
  auto expr = equality();
  while (look->tag == lexer::Tag::AND) {
    auto tok = look;
    auto loc = tok->loc;
    move();
    expr = std::make_shared<ast::And>(loc, tok, expr, equality());
  }
  return expr;
}

// Equality
sptr<ast::Expr> Parser::equality() {
  auto expr = rel();
  while (look->tag == lexer::Tag::EQ || look->tag == lexer::Tag::NE) {
    auto tok = look;
    auto loc = tok->loc;
    move();
    expr = std::make_shared<ast::Rel>(loc, tok, expr, rel());
  }
  return expr;
}

// Relational
sptr<ast::Expr> Parser::rel() {
  auto expr = arith();
  while (look->tag == lexer::Tag::LESS || look->tag == lexer::Tag::LE ||
         look->tag == lexer::Tag::GREATER || look->tag == lexer::Tag::GE) {
    auto tok = look;
    auto loc = tok->loc;
    move();
    expr = std::make_shared<ast::Rel>(loc, tok, expr, arith());
  }
  return expr;
}

// Addition / subtraction
sptr<ast::Expr> Parser::arith() {
  auto expr = term();
  while (look->tag == lexer::Tag::OP_PLUS ||
         look->tag == lexer::Tag::OP_MINUS) {
    auto tok = look;
    auto loc = tok->loc;
    move();
    expr = std::make_shared<ast::Arith>(loc, tok, expr, term());
  }
  return expr;
}

// Multiplication / division
sptr<ast::Expr> Parser::term() {
  auto expr = unary();
  while (look->tag == lexer::Tag::OP_MUL || look->tag == lexer::Tag::OP_DIV) {
    auto tok = look;
    auto loc = tok->loc;
    move();
    expr = std::make_shared<ast::Arith>(loc, tok, expr, unary());
  }
  return expr;
}

// Unary
sptr<ast::Expr> Parser::unary() {
  if (look->tag == lexer::Tag::MINUS || look->tag == lexer::Tag::UnaryNOT) {
    auto tok = look;
    auto loc = tok->loc;
    move();
    if (tok->tag == lexer::Tag::UnaryNOT)
      return std::make_shared<ast::Not>(loc, tok, unary());
    else
      return std::make_shared<ast::Unary>(loc, tok, unary());
  }
  return factor();
}

// Factor
sptr<ast::Expr> Parser::factor() {
  using namespace lexer;
  auto loc = look->loc;

  if (look->tag == Tag::NUM || look->tag == Tag::REAL) {
    auto w = std::make_shared<Word>(look->lexeme, look->tag, loc);
    auto node = std::make_shared<ast::Constant>(loc, w);
    move();
    return node;
  }

  if (look->tag == Tag::TRUE_ || look->tag == Tag::FALSE_) {
    auto w = std::make_shared<Word>(look->lexeme, look->tag, loc);
    auto node = std::make_shared<ast::Constant>(loc, w);
    move();
    return node;
  }

  if (look->tag == Tag::ID) {
    std::string name = look->lexeme;
    move();
    auto entry = top->get(name);
    if (!entry) throw std::runtime_error("Undeclared variable: " + name);

    auto varNode = std::make_shared<ast::IdExpr>(
        loc, std::static_pointer_cast<symbols::Id>(entry));

    if (look->tag == sym('[')) {
      move();
      auto indexExpr = assign();
      match(']');
      return std::make_shared<ast::Access>(loc, varNode, indexExpr);
    }
    return varNode;
  }

  if (look->tag == sym('(')) {
    move();
    auto e = assign();
    match(')');
    return e;
  }

  throw std::runtime_error("Unexpected token in factor: " + look->toString());
}

}  // namespace parser
