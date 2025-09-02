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
  else {
    std::string str =
        "Syntax error: unexpected token '" + look->toString() + "'";
    error(str, look->loc);
  }
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
  sptr<symbols::Env> savedEnv = top;
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
    sptr<symbols::Type> p = type();
    sptr<lexer::Token> tok = look;  // save token
    match(lexer::Tag::ID);

    auto id = std::make_shared<symbols::Id>(tok->lexeme, p, bytesUsed);
    top->put(tok->lexeme, id);
    bytesUsed += p->width;

    if (look->tag == lexer::Tag::ASSIGN) {
      move();
      sptr<ast::Expr> initExpr = assign();  // save init expr if needed
    }
    match(';');
  }
}

// Parse a type
sptr<symbols::Type> Parser::type() {
  auto tt = std::dynamic_pointer_cast<lexer::TypeToken>(look);
  if (!tt) {
    std::string str = "Expected type, got: " + look->toString();
    error(str, look->loc);
  }
  match(lexer::Tag::BASIC);
  sptr<symbols::Type> p = tt->typeInfo;
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
    sptr<ast::Expr> e = assign();
    match(';');
  } else {
    std::string str = "Unknown statement start: " + look->toString();
    SourceLocation loc = look->loc;
    error(str, loc);
  }
}

// Assignment expression
sptr<ast::Expr> Parser::assign() {
  sptr<ast::Expr> left = orExpr();
  if (look->tag == lexer::Tag::ASSIGN) {
    sptr<lexer::Token> tok = look;
    SourceLocation loc = tok->loc;
    move();
    sptr<ast::Expr> right = assign();
    return std::make_shared<ast::Op>(loc, tok, left, right);
  }
  return left;
}

// Logical OR
sptr<ast::Expr> Parser::orExpr() {
  sptr<ast::Expr> expr = andExpr();
  while (look->tag == lexer::Tag::OR) {
    sptr<lexer::Token> tok = look;
    SourceLocation loc = tok->loc;
    move();
    expr = std::make_shared<ast::Or>(loc, tok, expr, andExpr());
  }
  return expr;
}

// Logical AND
sptr<ast::Expr> Parser::andExpr() {
  sptr<ast::Expr> expr = equality();
  while (look->tag == lexer::Tag::AND) {
    sptr<lexer::Token> tok = look;
    SourceLocation loc = tok->loc;
    move();
    expr = std::make_shared<ast::And>(loc, tok, expr, equality());
  }
  return expr;
}

// Equality
sptr<ast::Expr> Parser::equality() {
  sptr<ast::Expr> expr = rel();
  while (look->tag == lexer::Tag::EQ || look->tag == lexer::Tag::NE) {
    sptr<lexer::Token> tok = look;
    SourceLocation loc = tok->loc;
    move();
    expr = std::make_shared<ast::Rel>(loc, tok, expr, rel());
  }
  return expr;
}

// Relational
sptr<ast::Expr> Parser::rel() {
  sptr<ast::Expr> expr = arith();
  while (look->tag == lexer::Tag::LESS || look->tag == lexer::Tag::LE ||
         look->tag == lexer::Tag::GREATER || look->tag == lexer::Tag::GE) {
    sptr<lexer::Token> tok = look;
    SourceLocation loc = tok->loc;
    move();
    expr = std::make_shared<ast::Rel>(loc, tok, expr, arith());
  }
  return expr;
}

// Addition / subtraction
sptr<ast::Expr> Parser::arith() {
  sptr<ast::Expr> expr = term();
  while (look->tag == lexer::Tag::OP_PLUS ||
         look->tag == lexer::Tag::OP_MINUS) {
    sptr<lexer::Token> tok = look;
    SourceLocation loc = tok->loc;
    move();
    expr = std::make_shared<ast::Arith>(loc, tok, expr, term());
  }
  return expr;
}

// Multiplication / division
sptr<ast::Expr> Parser::term() {
  sptr<ast::Expr> expr = unary();
  while (look->tag == lexer::Tag::OP_MUL || look->tag == lexer::Tag::OP_DIV) {
    sptr<lexer::Token> tok = look;
    SourceLocation loc = tok->loc;
    move();
    expr = std::make_shared<ast::Arith>(loc, tok, expr, unary());
  }
  return expr;
}

// Unary
sptr<ast::Expr> Parser::unary() {
  if (look->tag == lexer::Tag::MINUS || look->tag == lexer::Tag::UnaryNOT) {
    sptr<lexer::Token> tok = look;
    SourceLocation loc = tok->loc;
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
  SourceLocation loc = look->loc;

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
    sptr<symbols::Id> entry = top->get(name);
    if (!entry) {
      std::string str = "Undeclared variable: " + name;
      error(str, loc);
    }

    auto varNode = std::make_shared<ast::IdExpr>(
        loc, std::static_pointer_cast<symbols::Id>(entry));

    if (look->tag == sym('[')) {
      move();
      sptr<ast::Expr> indexExpr = assign();
      match(']');
      return std::make_shared<ast::Access>(loc, varNode, indexExpr);
    }
    return varNode;
  }

  if (look->tag == sym('(')) {
    move();
    sptr<ast::Expr> e = assign();
    match(')');
    return e;
  }

  std::string str = "Unexpected token in factor: " + look->toString();
  error(str, loc);
}

void error(const std::string& s, const SourceLocation& loc) {
  throw std::runtime_error("Line " + std::to_string(loc.line) + ", column " +
                           std::to_string(loc.column) + " " + s);
}

}  // namespace parser
