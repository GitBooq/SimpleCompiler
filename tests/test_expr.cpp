#include <gtest/gtest.h>

#include "Array.hpp"
#include "Expr.hpp"
#include "IEmitter.hpp"
#include "Token.hpp"
#include "Type.hpp"
#include "Word.hpp"

using namespace ast;
using namespace lexer;
using namespace symbols;

// Dummy Expr
struct DummyExpr : public Expr {
  std::string name;
  DummyExpr(std::string n, sptr<Type> t) : Expr({1, 1}), name(std::move(n)) {
    exprType = t;
  }
  std::string emit(emit::IEmitter&) const override { return name; }
};

// Mock IEmitter
struct MockEmitter : public emit::IEmitter {
  std::string emitBinaryOp(const std::string& l, const sptr<lexer::Token>& op,
                           const std::string& r) override {
    return "(" + l + " " + op->lexeme + " " + r + ")";
  }
  std::string emitUnaryOp(const sptr<lexer::Token>& op,
                          const std::string& e) override {
    return "(" + op->lexeme + e + ")";
  }
  std::string emitLoadConst(const sptr<lexer::Token>& v) override {
    return v->lexeme;
  }
  std::string emitTemp(int n) override { return "t" + std::to_string(n); }
  std::string emitArrayAccess(const std::string& a,
                              const std::string& i) override {
    return a + "[" + i + "]";
  }

  /* Mock stubs (for Statements) */
  virtual void emitIf(const std::string& condLabel,
                      const std::function<void()>& thenBlock) {};
  virtual void emitIfElse(const std::string& condLabel,
                          const std::function<void()>& thenBlock,
                          const std::function<void()>& elseBlock) {};
  virtual void emitWhile(const std::function<std::string()>& condGen,
                         const std::function<void()>& bodyGen) {};
  virtual void emitDoWhile(const std::function<void()>& bodyGen,
                           const std::function<std::string()>& condGen) {};
  virtual void emitBreak() {};
  virtual void emitAssign(const std::string& target, const std::string& value) {
  };
  virtual void emitArrayAssign(const std::string& arr, const std::string& idx,
                               const std::string& value) {};
  /*-------------------------------------------------------------------------*/
};

// Tests

TEST(ConstantTests, IntLiteralHasIntType) {
  auto w = std::make_shared<Word>("42", Tag::NUM);
  Constant c({1, 1}, w);
  EXPECT_EQ(c.exprType, Type::Int);

  MockEmitter em;
  EXPECT_EQ(c.emit(em), "42");
}

TEST(ConstantTests, BoolLiteralHasBoolType) {
  auto w = std::make_shared<Word>("true", Tag::TRUE_);
  Constant c({1, 1}, w);
  EXPECT_EQ(c.exprType, Type::Bool);

  MockEmitter em;
  EXPECT_EQ(c.emit(em), "true");
}

TEST(ArithTests, IntPlusFloatGivesFloatAndEmit) {
  auto lhs = std::make_shared<DummyExpr>("x", Type::Int);
  auto rhs = std::make_shared<DummyExpr>("y", Type::Float);
  auto tok = std::make_shared<Token>(Tag::OP_PLUS, "+");
  Arith add({1, 1}, tok, lhs, rhs);

  EXPECT_EQ(add.exprType, Type::Float);

  MockEmitter em;
  EXPECT_EQ(add.emit(em), "(x + y)");
}

TEST(LogicalTests, BoolAndBoolGivesBool) {
  auto b1 = std::make_shared<DummyExpr>("b1", Type::Bool);
  auto b2 = std::make_shared<DummyExpr>("b2", Type::Bool);
  auto tok = std::make_shared<Token>(Tag::AND, "&&");
  Logical andNode({1, 1}, tok, b1, b2);

  EXPECT_EQ(andNode.exprType, Type::Bool);

  MockEmitter em;
  EXPECT_EQ(andNode.emit(em), "(b1 && b2)");
}

TEST(UnaryTests, NotExpressionIsBool) {
  auto b1 = std::make_shared<DummyExpr>("flag", Type::Bool);
  auto tok = std::make_shared<Token>(Tag::UnaryNOT, "!");
  Not notNode({1, 1}, tok, b1);

  EXPECT_EQ(notNode.exprType, Type::Bool);

  MockEmitter em;
  EXPECT_EQ(notNode.emit(em), "(!flag)");
}

TEST(TempTests, TempEmit) {
  Temp t({1, 1}, 7, Type::Int);
  EXPECT_EQ(t.exprType, Type::Int);

  MockEmitter em;
  EXPECT_EQ(t.emit(em), "t7");
}

TEST(AccessTests, ArrayElementTypeAndEmit) {
  auto arrType = std::make_shared<Array>(5, Type::Int);
  auto arrExpr = std::make_shared<DummyExpr>("arr", arrType);
  auto idxExpr = std::make_shared<DummyExpr>("i", Type::Int);
  Access acc({1, 1}, arrExpr, idxExpr);

  EXPECT_EQ(acc.exprType, Type::Int);

  MockEmitter em;
  EXPECT_EQ(acc.emit(em), "arr[i]");
}
