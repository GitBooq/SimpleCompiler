#include <gtest/gtest.h>

#include "Array.hpp"
#include "Expr.hpp"
#include "IEmitter.hpp"
#include "Stmt.h"
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
  /* Expressions */
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

  // --- Statements ---
  std::vector<std::string> log;

  void emitIf(const std::string& condLabel,
              const std::function<void()>& thenBlock) override {
    log.push_back("If(" + condLabel + ")");
    thenBlock();
  }

  void emitIfElse(const std::string& condLabel,
                  const std::function<void()>& thenBlock,
                  const std::function<void()>& elseBlock) override {
    log.push_back("IfElse(" + condLabel + ")");
    thenBlock();
    elseBlock();
  }

  void emitWhile(const std::function<std::string()>& condGen,
                 const std::function<void()>& bodyGen) override {
    log.push_back("While(" + condGen() + ")");
    bodyGen();
  }

  void emitDoWhile(const std::function<void()>& bodyGen,
                   const std::function<std::string()>& condGen) override {
    log.push_back("DoWhile(" + condGen() + ")");
    bodyGen();
  }

  void emitBreak() override { log.push_back("Break"); }

  void emitAssign(const std::string& target,
                  const std::string& value) override {
    log.push_back("Assign(" + target + "," + value + ")");
  }

  void emitArrayAssign(const std::string& arr, const std::string& idx,
                       const std::string& value) override {
    log.push_back("ArrayAssign(" + arr + "," + idx + "," + value + ")");
  }
  /*-------------------------------------------------------------------------*/
};

/* Expressions Tests */
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

/* Statements Tests */

TEST(StmtTests, BreakCallsEmitBreak) {
  MockEmitter em;
  ast::Break br({1, 1});
  br.emit(em);
  EXPECT_EQ(em.log, std::vector<std::string>{"Break"});
}

TEST(StmtTests, IfCallsEmitIfAndThenBody) {
  MockEmitter em;
  auto cond = std::make_shared<DummyExpr>("cond", Type::Bool);
  auto thenStmt = std::make_shared<ast::Break>(SourceLocation{1, 1});
  ast::If ifNode({1, 1}, cond, thenStmt);

  ifNode.emit(em);

  ASSERT_EQ(em.log.size(), 2);
  EXPECT_EQ(em.log[0], "If(cond)");
  EXPECT_EQ(em.log[1], "Break");
}

TEST(StmtTests, ElseCallsEmitIfElseAndBranches) {
  MockEmitter em;
  auto cond = std::make_shared<DummyExpr>("flag", Type::Bool);
  auto thenStmt = std::make_shared<ast::Break>(SourceLocation{1, 1});
  auto elseStmt = std::make_shared<ast::Break>(SourceLocation{1, 1});
  ast::Else elseNode({1, 1}, cond, thenStmt, elseStmt);

  elseNode.emit(em);

  ASSERT_EQ(em.log.size(), 3);
  EXPECT_EQ(em.log[0], "IfElse(flag)");
  EXPECT_EQ(em.log[1], "Break");
  EXPECT_EQ(em.log[2], "Break");
}

TEST(StmtTests, WhileCallsEmitWhile) {
  MockEmitter em;
  auto cond = std::make_shared<DummyExpr>("ok", Type::Bool);
  auto body = std::make_shared<ast::Break>(SourceLocation{1, 1});
  ast::While loopNode({1, 1}, cond, body);

  loopNode.emit(em);

  ASSERT_EQ(em.log.size(), 2);
  EXPECT_EQ(em.log[0], "While(ok)");
  EXPECT_EQ(em.log[1], "Break");
}

TEST(StmtTests, DoWhileCallsEmitDoWhile) {
  MockEmitter em;
  auto cond = std::make_shared<DummyExpr>("ready", Type::Bool);
  auto body = std::make_shared<ast::Break>(SourceLocation{1, 1});
  ast::Do node({1, 1}, body, cond);

  node.emit(em);

  ASSERT_EQ(em.log.size(), 2);
  EXPECT_EQ(em.log[0], "DoWhile(ready)");
  EXPECT_EQ(em.log[1], "Break");
}

TEST(StmtTests, SetCallsEmitAssign) {
  MockEmitter em;
  auto lhs = std::make_shared<DummyExpr>("x", Type::Int);
  auto rhs = std::make_shared<DummyExpr>("42", Type::Int);
  ast::Set node({1, 1}, lhs, rhs);

  node.emit(em);

  EXPECT_EQ(em.log, std::vector<std::string>{"Assign(x,42)"});
}

TEST(StmtTests, SetElemCallsEmitArrayAssign) {
  MockEmitter em;
  auto arrAccess = std::make_shared<DummyExpr>("arr[0]", Type::Int);
  auto value = std::make_shared<DummyExpr>("99", Type::Int);
  ast::SetElem node({1, 1}, arrAccess, value);

  node.emit(em);

  EXPECT_EQ(em.log, std::vector<std::string>{"ArrayAssign(arr[0],,99)"});
}