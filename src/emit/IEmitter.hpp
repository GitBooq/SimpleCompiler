/*
* File: IEmitter.hpp
Emitter interface.
*/

#pragma once
#include <functional>
#include <string>

#include "sptr.h"

/* Forward declarations */
namespace lexer {
struct Token;
}  // namespace lexer
namespace ast {
struct Expr;
struct Stmt;
}  // namespace ast

namespace emit {
// TODO: Divide IEmitter on two interfaces for Exprs and Stmts
struct IEmitter {
  virtual ~IEmitter() = default;

  /* Expressions */

  /// Constant: creates a temporary variable and initializes it with the value
  virtual std::string emitLoadConst(const sptr<lexer::Token>& tok) = 0;

  /// Unary operator
  virtual std::string emitUnaryOp(const sptr<lexer::Token>& op,
                                  const std::string& operand) = 0;

  /// Binary operator
  virtual std::string emitBinaryOp(const std::string& lhs,
                                   const sptr<lexer::Token>& op,
                                   const std::string& rhs) = 0;

  /// Array access operator
  virtual std::string emitArrayAccess(const std::string& arr,
                                      const std::string& idx) = 0;

  /// Temp variable
  virtual std::string emitTemp(int number) = 0;
  /*-------------------------------------------------------------------------*/

  /* Statements */

  /// if(cond) w/o else
  virtual void emitIf(const std::string& condLabel,
                      const std::function<void()>& thenBlock) = 0;

  /// if(cond) ... else ...
  virtual void emitIfElse(const std::string& condLabel,
                          const std::function<void()>& thenBlock,
                          const std::function<void()>& elseBlock) = 0;

  /// while(cond) {...}
  virtual void emitWhile(const std::function<std::string()>& condGen,
                         const std::function<void()>& bodyGen) = 0;

  /// do {...} while(cond);
  virtual void emitDoWhile(const std::function<void()>& bodyGen,
                           const std::function<std::string()>& condGen) = 0;

  /// break;
  virtual void emitBreak() = 0;

  /// x = expr;
  virtual void emitAssign(const std::string& target,
                          const std::string& value) = 0;

  /// arr[idx] = expr;
  virtual void emitArrayAssign(const std::string& arr, const std::string& idx,
                               const std::string& value) = 0;
  /*-------------------------------------------------------------------------*/
};
}  // namespace emit