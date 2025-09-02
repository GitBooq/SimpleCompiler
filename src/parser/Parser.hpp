/**
 * @file Parser.hpp
 * @brief Recursive descent parser that builds an AST from tokens provided by
 * the Lexer.
 *
 * The Parser consumes tokens produced by the Lexer, validates their syntax
 * according to the grammar, and builds an Abstract Syntax Tree (AST) using
 * classes from the `ast` module (Expr, Op, Arith, Rel, etc.).
 *
 * Grammar supports variable declarations (with optional initialization),
 * statements, and expressions with full precedence:
 * assignment, logical OR/AND, equality, relational, arithmetic, unary, and
 * factor.
 */

#pragma once
#include <memory>

#include "Array.hpp"
#include "Env.hpp"
#include "Expr.hpp"
#include "Id.hpp"
#include "Lexer.hpp"
#include "Type.hpp"
#include "TypeToken.hpp"

namespace parser {

/**
 * @class Parser
 * @brief Implements a hand-written recursive descent parser for the toy
 * language.
 */
class Parser {
 public:
  /**
   * @brief Construct a new Parser instance.
   * @param l Shared pointer to the Lexer that provides tokens.
   */
  Parser::Parser(const std::shared_ptr<lexer::Lexer>& l)
      : lex(l), top(std::make_shared<symbols::Env>()), bytesUsed(0) {
    move();
  }

  /**
   * @brief Entry point for parsing. Parses a complete program.
   */
  void program();

 private:
  sptr<lexer::Lexer> lex;   ///< The associated lexer instance
  sptr<lexer::Token> look;  ///< Lookahead token
  sptr<symbols::Env> top;   ///< Current symbol table environment
  int bytesUsed;            ///< Accumulated memory usage for variables

  // === Basic methods ===

  /**
   * @brief Advance to the next token from the lexer.
   */
  void move();

  /**
   * @brief Match the current token's tag against the expected tag, or throw an
   * error.
   * @param t Expected token tag.
   */
  void match(lexer::Tag t);

  /**
   * @brief Overload of match for single-character tokens.
   * @param ch Character to match.
   */
  void match(char ch);

  // === Program structure ===

  /**
   * @brief Parse a code block: '{' declarations statements '}' with its own
   * scope.
   * @return nullptr (placeholder for integration with code generation).
   */
  void* block();

  /**
   * @brief Parse variable declarations (with optional initialization).
   */
  void decls();

  /**
   * @brief Parse a type specification (primitive or array).
   * @return Parsed type as shared pointer.
   */
  sptr<symbols::Type> type();

  /**
   * @brief Parse array dimensions recursively.
   * @param p Element type.
   * @return Array type with given element type and dimensions.
   */
  sptr<symbols::Type> dims(sptr<symbols::Type> p);

  /**
   * @brief Parse a sequence of statements until '}' or EOF.
   */
  void stmts();

  /**
   * @brief Parse a single statement.
   */
  void stmt();

  // === Expressions ===

  /**
   * @brief Parse an assignment expression (right-associative).
   * @return AST node representing the expression.
   */
  sptr<ast::Expr> assign();

  /**
   * @brief Parse logical OR expressions.
   */
  sptr<ast::Expr> orExpr();

  /**
   * @brief Parse logical AND expressions.
   */
  sptr<ast::Expr> andExpr();

  /**
   * @brief Parse equality expressions (==, !=).
   */
  sptr<ast::Expr> equality();

  /**
   * @brief Parse relational expressions (<, <=, >, >=).
   */
  sptr<ast::Expr> rel();

  /**
   * @brief Parse addition and subtraction.
   */
  sptr<ast::Expr> arith();

  /**
   * @brief Parse multiplication and division.
   */
  sptr<ast::Expr> term();

  /**
   * @brief Parse unary operations (-, !).
   */
  sptr<ast::Expr> unary();

  /**
   * @brief Parse factors: literals, identifiers, array access, or grouped
   * expressions.
   */
  sptr<ast::Expr> factor();
};

/**
 * @brief Error handling
 * @param s String containing error message.
 * @param loc Location where the error occurred at.
 */
void error(const std::string& s, const SourceLocation& loc);

}  // namespace parser
