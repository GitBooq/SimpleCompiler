/**
 * @file Expr.cpp
 * @brief Implementation for expressions for Abstract Syntax Tree (AST).
 */

#include "Expr.hpp"

#include <string>

#include "Array.hpp"
#include "IEmitter.hpp"
#include "Token.hpp"
#include "Type.hpp"
#include "Word.hpp"

using emit::IEmitter;

namespace ast {
// Logical ctor
Logical::Logical(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> l,
                 sptr<Expr> r)
    : Op(loc, std::move(tok), std::move(l), std::move(r)) {
  // Type Check
  if (this->lhs->exprType != symbols::Type::Bool ||
      this->rhs->exprType != symbols::Type::Bool) {
    throw std::runtime_error("Logical operations require bool operands");
  }

  // Always bool
  exprType = symbols::Type::Bool;
}

// Binary Ops
std::string Op::emit(IEmitter& out) const {
  auto leftName = lhs->emit(out);
  auto rightName = rhs->emit(out);
  return out.emitBinaryOp(leftName, op_tok, rightName);
}

// Arith ctor
Arith::Arith(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> l,
             sptr<Expr> r)
    : Op(loc, tok, l, r) {
  exprType = symbols::Type::max(lhs->exprType, rhs->exprType);
  if (!exprType)
    throw std::runtime_error("Arithmetic operands must be numeric");
}

// Rel ctor
Rel::Rel(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> l, sptr<Expr> r)
    : Op(loc, tok, l, r) {
  // Always bool
  exprType = symbols::Type::Bool;
}

// Constant
std::string Constant::emit(IEmitter& out) const {
  return out.emitLoadConst(value);
}

// Constant ctor
Constant::Constant(SourceLocation loc, sptr<lexer::Word> v)
    : Expr(loc), value(std::move(v)) {
  switch (value->tag) {
    case lexer::Tag::NUM:
      exprType = symbols::Type::Int;
      break;
    case lexer::Tag::REAL:
      exprType = symbols::Type::Float;
      break;
    case lexer::Tag::TRUE_:
    case lexer::Tag::FALSE_:
      exprType = symbols::Type::Bool;
      break;
    default:
      exprType = nullptr;  // TODO Error
      break;
  }
}

// Temp
std::string Temp::emit(IEmitter& out) const { return out.emitTemp(number); }

// Temp ctor
Temp::Temp(SourceLocation loc, int n, sptr<symbols::Type> t)
    : Expr(loc), number(n) {
  exprType = t;
}

// Unary Op
std::string Unary::emit(IEmitter& out) const {
  auto exprName = expr->emit(out);
  return out.emitUnaryOp(op_tok, exprName);
}

// Not ctor
Not::Not(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> e)
    : Unary(loc, tok, e) {
  // Always bool
  exprType = symbols::Type::Bool;
}

// Access array (a[i])
std::string Access::emit(IEmitter& out) const {
  auto arrName = array->emit(out);
  auto idxName = index->emit(out);
  return out.emitArrayAccess(arrName, idxName);
}

// Access ctor
Access::Access(SourceLocation loc, sptr<Expr> arr, sptr<Expr> idx)
    : Expr(loc), array(std::move(arr)), index(std::move(idx)) {
  // Check Array type
  if (auto arrType =
          std::dynamic_pointer_cast<symbols::Array>(array->exprType)) {
    exprType = arrType->of;
  } else {
    exprType = nullptr;  // TODO Error
  }
}
}  // namespace ast