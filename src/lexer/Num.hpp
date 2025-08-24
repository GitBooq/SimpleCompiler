/**
 * @file Num.hpp
 * @brief Token class for integer numbers.
 */
#pragma once
#include "Token.hpp"

namespace lexer {

/**
 * @brief Represents an integer number token.
 */
struct Num : public Token {
  int value; /**< Numeric value */

  /**
   * @brief Creates an integer token.
   * @param v Integer value.
   */
  explicit Num(int v) : Token(Tag::NUM, std::to_string(v)), value(v) {}
};

}  // namespace lexer
