/**
 * @file Lexer.cpp
 * @brief Implementation of the Lexer class methods.
 */
#include "Lexer.hpp"

#include <cctype>

namespace lexer {

Lexer::Lexer(std::istream& in) : input(in), lineNumber(1) {
  // Register reserved keywords
  reserve(std::make_shared<Word>("if", Tag::IF));
  reserve(std::make_shared<Word>("else", Tag::ELSE));
  reserve(std::make_shared<Word>("while", Tag::WHILE));
  reserve(std::make_shared<Word>("do", Tag::DO));
  reserve(std::make_shared<Word>("break", Tag::BREAK));
  reserve(Word::True);
  reserve(Word::False);
  reserve(std::make_shared<Word>("int", Tag::BASIC));
  reserve(std::make_shared<Word>("float", Tag::BASIC));
  reserve(std::make_shared<Word>("bool", Tag::BASIC));
  reserve(std::make_shared<Word>("char", Tag::BASIC));
}

void Lexer::reserve(const sptr<Word>& w) { words.insert({w->lexeme, w}); }

char Lexer::readch() {
  char c;
  if (input.get(c)) return c;
  return '\0';  // Null char to indicate EOF
}

bool Lexer::readch(char c) {
  char ch = readch();
  if (ch != c) {
    if (ch != '\0') input.putback(ch);
    return false;
  }
  return true;
}

sptr<Token> Lexer::scan() {
  // Skip whitespace and track line breaks
  for (;;) {
    if (!input.good()) return std::make_shared<Token>(Tag(0));  // End of input
    char c = readch();
    if (c == ' ' || c == '\t')
      continue;
    else if (c == '\n') {
      ++lineNumber;
      continue;
    } else {
      input.putback(c);
      break;
    }
  }

  char c = readch();

  // Handle two-character operators
  switch (c) {
    case '&':
      if (readch('&'))
        return Word::And;
      else
        return std::make_shared<Token>(Tag('&'));
    case '|':
      if (readch('|'))
        return Word::Or;
      else
        return std::make_shared<Token>(Tag('|'));
    case '=':
      if (readch('='))
        return Word::eq;
      else
        return std::make_shared<Token>(Tag('='));
    case '!':
      if (readch('='))
        return Word::ne;
      else
        return std::make_shared<Token>(Tag('!'));
    case '<':
      if (readch('='))
        return Word::le;
      else
        return std::make_shared<Token>(Tag('<'));
    case '>':
      if (readch('='))
        return Word::ge;
      else
        return std::make_shared<Token>(Tag('>'));
  }

  // Handle numeric literals
  if (std::isdigit(c)) {
    int value = 0;
    bool isFloat = false;
    float fValue = 0.0f;
    float divisor = 10.0f;

    // Integer part
    do {
      value = 10 * value + (c - '0');
      c = readch();
    } while (std::isdigit(c));

    // Fractional part
    if (c == '.') {
      isFloat = true;
      c = readch();
      while (std::isdigit(c)) {
        fValue += (c - '0') / divisor;
        divisor *= 10.0f;
        c = readch();
      }
    }

    if (c != '\0') input.putback(c);
    if (isFloat) return std::make_shared<Real>(value + fValue);
    return std::make_shared<Num>(value);
  }

  // Handle identifiers and keywords
  if (std::isalpha(c)) {
    std::string s;
    do {
      s += c;
      c = readch();
    } while (std::isalnum(c));
    if (c != '\0') input.putback(c);

    auto it = words.find(s);
    if (it != words.end()) return it->second;

    sptr<Word> w = std::make_shared<Word>(s, Tag::ID);
    reserve(w);
    return w;
  }

  // Return token for single-character symbols
  return std::make_shared<Token>(Tag(c));
}

}  // namespace lexer
