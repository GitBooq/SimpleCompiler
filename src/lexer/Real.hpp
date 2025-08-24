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
  explicit Real(float v) : Token(Tag::REAL, std::to_string(v)), value(v) {}
};

}  // namespace lexer
