/*
* File: IEmitter.hpp
Emitter interface.
*/

#pragma once
#include <string>

#include "sptr.h"

/* Forward declarations */
namespace lexer {
struct Token;
}  // namespace lexer
namespace ast {
struct Expr;
}  // namespace ast

namespace emit {
struct IEmitter {
  virtual ~IEmitter() = default;

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
};
}  // namespace emit