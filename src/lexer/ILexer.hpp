/**
 * @file ILexer.hpp
 * @brief Interface for the lexer.
 */
#pragma once
#include <memory>

#include "Token.hpp"
#include "sptr.h"

namespace lexer {

/**
 * @brief Interface to be implemented by lexer classes.
 */
struct ILexer {
  virtual ~ILexer() = default;

  /**
   * @brief Reads the next token from the input stream.
   * @return Token object.
   */
  virtual sptr<Token> scan() = 0;

  /**
   * @brief Returns the current line number.
   */
  virtual int line() const = 0;
};

}  // namespace lexer
