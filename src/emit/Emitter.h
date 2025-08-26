/*
* File: Emitter.h
 Emitter classes
*/
#pragma once
#include "IEmitter.hpp"

namespace emit {
/*
  Generation of human-readable text(C-style pseudocode).
  The methods return strings with ready-made code.
*/
struct TextEmitter : IEmitter {
  /// Constant: creates a temporary variable and initializes it with the value
  std::string emitLoadConst(const sptr<lexer::Token>& value) override;

  /// Unary operator
  std::string emitUnaryOp(const sptr<lexer::Token>& op,
                          const std::string& operand) override;

  /// Binary operator
  std::string emitBinaryOp(const std::string& lhs, const sptr<lexer::Token>& op,
                           const std::string& rhs) override;

  /// Array access operator
  std::string emitArrayAccess(const std::string& arr,
                              const std::string& idx) override;

  /// Temp variable
  std::string emitTemp(int number) override;
};

// TODO AsmEmitter
// TODO IRBuilderEmitter (LLVM IR)
}  // namespace emit
