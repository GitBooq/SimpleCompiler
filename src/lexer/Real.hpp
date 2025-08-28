/**
 * @file Real.hpp
 * @brief Token class for floating-point numbers.
 */
#pragma once
#include "Token.hpp"

namespace lexer {

/**
 * @brief Represents a floating-point number token.
 */
struct Real : public Token {
  float value; /**< Floating-point value */

  /**
   * @brief Creates a floating-point token.
   * @param v Floating-point value.
   */
  explicit Real(float v, SourceLocation loc = {0, 0})
      : Token(Tag::REAL, std::to_string(v), loc), value(v) {}
};

}  // namespace lexer
