/**
 * @file Word.hpp
 * @brief Token class for identifiers and keywords.
 */
#pragma once

#include "Token.hpp"
#include "sptr.h"

#ifdef _WIN32     // Only on Windows
#ifdef UNIT_TEST  // Only when building tests
#include "debugWinapi.h"
#endif
#endif

namespace lexer {

/**
 * @brief Represents a keyword or identifier token.
 */
struct Word : public Token {
  /**
   * @brief Constructs a Word token.
   * @param s String value.
   * @param tag Token tag.
   */
  Word(std::string s, Tag tag, SourceLocation loc = {0, 0})
      : Token(tag, std::move(s), loc) {}

#ifdef _WIN32     // Only on Windows
#ifdef UNIT_TEST  // Only when building tests
  Word() : Token(Tag::ID, "", {1, 1}) {
    TRACE("TRACE");
  }  // temporary for debug
#endif
#endif

  /// Predefined constants for keywords and operators.
  static const sptr<Word> And, Or, eq, ne, le, ge, True, False, temp, minus;
};

// Static constant definitions
inline const sptr<Word> Word::And = std::make_shared<Word>("&&", Tag::AND);
inline const sptr<Word> Word::Or = std::make_shared<Word>("||", Tag::OR);
inline const sptr<Word> Word::eq = std::make_shared<Word>("==", Tag::EQ);
inline const sptr<Word> Word::ne = std::make_shared<Word>("!=", Tag::NE);
inline const sptr<Word> Word::le = std::make_shared<Word>("<=", Tag::LE);
inline const sptr<Word> Word::ge = std::make_shared<Word>(">=", Tag::GE);
inline const sptr<Word> Word::True = std::make_shared<Word>("true", Tag::TRUE_);
inline const sptr<Word> Word::False =
    std::make_shared<Word>("false", Tag::FALSE_);
inline const sptr<Word> Word::temp = std::make_shared<Word>("t", Tag::TEMP);
inline const sptr<Word> Word::minus = std::make_shared<Word>("-", Tag::MINUS);

}  // namespace lexer
