/**
 * @file Token.hpp
 * @brief Base class representing a token.
 */
#pragma once
#include <string>

#include "Tag.hpp"

namespace lexer {

/**
 * @brief Represents a token returned by the lexer.
 */
struct Token {
  Tag tag;            /**< Token tag */
  std::string lexeme; /**< Token text representation */

  /**
   * @brief Constructs a token.
   * @param t Token tag.
   * @param l Token lexeme (defaults to empty).
   */
  Token(Tag t, std::string l = "") : tag(t), lexeme(std::move(l)) {}
  Token(Tag t, char c) : tag(t), lexeme(std::string(1, c)) {}
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
