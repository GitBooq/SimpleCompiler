/**
 * @file Lexer.hpp
 * @brief Lexer implementation.
 */
#pragma once
#include <istream>
#include <unordered_map>

#include "ILexer.hpp"
#include "Num.hpp"
#include "Real.hpp"
#include "Word.hpp"
#include "sptr.h"

namespace lexer {

/**
 * @brief Class for lexical analysis of source code.
 */
struct Lexer : public ILexer {
  std::istream& input;                                ///< Input data stream.
  int lineNumber;                                     ///< Current line number.
  std::unordered_map<std::string, sptr<Word>> words;  ///< Keyword table.

  /**
   * @brief Lexer constructor.
   * @param in Input data stream.
   */
  explicit Lexer(std::istream& in);

  /**
   * @brief Registers a keyword.
   * @param w Word object.
   */
  void reserve(const sptr<Word>& w);

  /// Reads a single character.
  char readch();

  /// Reads a character and checks it against the expected one.
  bool readch(char c);

  /// @copydoc ILexer::scan()
  sptr<Token> scan() override;

  /// @copydoc ILexer::line()
  int line() const override { return lineNumber; }
};

}  // namespace lexer
