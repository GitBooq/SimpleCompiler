/**
 * @file Token.hpp
 * @brief Base class representing a token.
 */
#pragma once
#include <string>

#include "ASTNode.hpp"
#include "Tag.hpp"

namespace lexer {

/**
 * @brief Represents a token returned by the lexer.
 */
struct Token {
  Tag tag;            /**< Token tag */
  std::string lexeme; /**< Token text representation */
  SourceLocation loc; /**< Line/column position in source */

  /**
   * @brief Constructs a token.
   * @param t Token tag.
   * @param l Token lexeme (defaults to empty).
   */
  Token(Tag t, std::string l = "", SourceLocation loc = {0, 0})
      : tag(t), lexeme(std::move(l)), loc(loc) {}

  Token(Tag t, char c, SourceLocation loc = {0, 0})
      : tag(t), lexeme(std::string(1, c)), loc(loc) {}
  virtual ~Token() = default;

  /**
   * @brief Returns the string representation of the token.
   * @return String for output.
   */
  virtual std::string toString() const {
    if (!lexeme.empty()) return lexeme;
    return std::to_string(static_cast<int>(tag));
  }
};

}  // namespace lexer
