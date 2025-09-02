/**
 * @file Lexer.cpp
 * @brief Implementation of the Lexer class methods.
 */
#include "Lexer.hpp"

#include <cctype>

#include "TypeToken.hpp"

namespace lexer {

Lexer::Lexer(std::istream& in) : input(in), loc{1, 1} {
  // Register reserved keywords
  reserve(std::make_shared<Word>("if", Tag::IF));
  reserve(std::make_shared<Word>("else", Tag::ELSE));
  reserve(std::make_shared<Word>("while", Tag::WHILE));
  reserve(std::make_shared<Word>("do", Tag::DO));
  reserve(std::make_shared<Word>("break", Tag::BREAK));
  reserve(Word::True);
  reserve(Word::False);
  reserve(std::make_shared<TypeToken>(symbols::Type::Int));
  reserve(std::make_shared<TypeToken>(symbols::Type::Float));
  reserve(std::make_shared<TypeToken>(symbols::Type::Bool));
  reserve(std::make_shared<TypeToken>(symbols::Type::Char));
}

void Lexer::reserve(const sptr<Word>& w) { words.insert({w->lexeme, w}); }

char Lexer::readch() {
  int& lineNumber = loc.line;
  int& columnNumber = loc.column;
  char c;
  if (input.get(c)) {
    if (c == '\n') {
      ++lineNumber;
      columnNumber = 0;
    } else {
      ++columnNumber;
    }
    return c;
  }
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
  // save current symbol location
  SourceLocation startLoc{loc.line, loc.column + 1};

  int& lineNumber = loc.line;
  int& columnNumber = loc.column;

  // Skip ws and control newlines
  for (;;) {
    if (!input.good()) return std::make_shared<Token>(Tag::END, "", startLoc);

    char c = readch();
    if (c == ' ' || c == '\t') {
      continue;
    } else if (c == '\n') {
      ++lineNumber;
      columnNumber = 0;
      startLoc = {lineNumber, columnNumber + 1};
      continue;
    } else {
      input.putback(c);
      break;
    }
  }

  char c = readch();

  // Operators and separators
  switch (c) {
    case '&':
      if (readch('&'))
        return std::make_shared<Word>(Word::And->lexeme, Word::And->tag,
                                      startLoc);
      return std::make_shared<Token>(Tag::bitAND, c, startLoc);
    case '|':
      if (readch('|'))
        return std::make_shared<Word>(Word::Or->lexeme, Word::Or->tag,
                                      startLoc);
      return std::make_shared<Token>(Tag::bitOR, c, startLoc);
    case '=':
      if (readch('='))
        return std::make_shared<Word>(Word::eq->lexeme, Word::eq->tag,
                                      startLoc);
      return std::make_shared<Token>(Tag::ASSIGN, c, startLoc);
    case '!':
      if (readch('='))
        return std::make_shared<Word>(Word::ne->lexeme, Word::ne->tag,
                                      startLoc);
      return std::make_shared<Token>(Tag::UnaryNOT, c, startLoc);
    case '<':
      if (readch('='))
        return std::make_shared<Word>(Word::le->lexeme, Word::le->tag,
                                      startLoc);
      return std::make_shared<Token>(Tag::LESS, c, startLoc);
    case '>':
      if (readch('='))
        return std::make_shared<Word>(Word::ge->lexeme, Word::ge->tag,
                                      startLoc);
      return std::make_shared<Token>(Tag::GREATER, c, startLoc);
    case '+':
      return std::make_shared<Token>(Tag::OP_PLUS, c, startLoc);
    case '-':
      return std::make_shared<Token>(Tag::OP_MINUS, c, startLoc);
    case '*':
      return std::make_shared<Token>(Tag::OP_MUL, c, startLoc);
    case '/':
      return std::make_shared<Token>(Tag::OP_DIV, c, startLoc);
  }

  // Numbers
  if (std::isdigit(c)) {
    int value = 0;
    bool isFloat = false;
    float fValue = 0.0f;
    float divisor = 10.0f;

    do {
      value = 10 * value + (c - '0');
      c = readch();
    } while (std::isdigit(c));

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
    if (isFloat) return std::make_shared<Real>(value + fValue, startLoc);
    return std::make_shared<Num>(value, startLoc);
  }

  // Identificators / keywords
  if (std::isalpha(c)) {
    std::string s;
    do {
      s += c;
      c = readch();
    } while (std::isalnum(c));
    if (c != '\0') input.putback(c);

    auto it = words.find(s);
    if (it != words.end()) {
      // keyword - return copy with location
      sptr<lexer::Word> tokenPtr = it->second;

      if (auto typeTok =
              std::dynamic_pointer_cast<lexer::TypeToken>(tokenPtr)) {
        // keyword is a base type, copy as TypeToken
        return std::make_shared<lexer::TypeToken>(typeTok->typeInfo, startLoc);
      }
      // else common keyword
      return std::make_shared<lexer::Word>(tokenPtr->lexeme, tokenPtr->tag,
                                           startLoc);
    }

    // new id
    auto w = std::make_shared<Word>(s, Tag::ID, startLoc);
    reserve(w);
    return w;
  }

  // else one-char symbols
  return std::make_shared<Token>(Tag(c), c, startLoc);
}

}  // namespace lexer
