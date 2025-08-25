#include <gtest/gtest.h>

#include <sstream>

#include "Lexer.hpp"

using namespace lexer;

TEST(LexerTest, RecognizesKeyword) {
  std::istringstream input("if else while");
  Lexer lex(input);

  sptr<Token> t1 = lex.scan();
  EXPECT_EQ(t1->tag, Tag::IF);

  sptr<Token> t2 = lex.scan();
  EXPECT_EQ(t2->tag, Tag::ELSE);

  sptr<Token> t3 = lex.scan();
  EXPECT_EQ(t3->tag, Tag::WHILE);
}

TEST(LexerTest, RecognizesInteger) {
  std::istringstream input("123");
  Lexer lex(input);

  sptr<Token> t = lex.scan();
  EXPECT_EQ(t->tag, Tag::NUM);
  EXPECT_EQ(std::dynamic_pointer_cast<Num>(t)->value, 123);
}

TEST(LexerTest, RecognizesFloat) {
  std::istringstream input("3.14");
  Lexer lex(input);

  sptr<Token> t = lex.scan();
  EXPECT_EQ(t->tag, Tag::REAL);
  EXPECT_FLOAT_EQ(std::dynamic_pointer_cast<Real>(t)->value, 3.14f);
  EXPECT_NEAR(std::dynamic_pointer_cast<Real>(t)->value, 3.14f, 1e-6f);
}

TEST(LexerTest, RecognizesIdentifier) {
  std::istringstream input("hello");
  Lexer lex(input);

  sptr<Token> t = lex.scan();
  EXPECT_EQ(t->tag, Tag::ID);
  EXPECT_EQ(t->lexeme, "hello");
}
