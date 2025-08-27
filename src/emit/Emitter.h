/*
* File: Emitter.h
 Emitter classes
*/
#pragma once
#include <string>

#include "IEmitter.hpp"

namespace emit {
/*
  Generation of human-readable text(C-style pseudocode).
  The methods for expressions return strings with ready-made code.
*/
struct TextEmitter : IEmitter {
  /* Expressions */

  /// \copydoc IEmitter::emitLoadConst
  std::string emitLoadConst(const sptr<lexer::Token>& value) override;

  /// \copydoc IEmitter::emitUnaryOp
  std::string emitUnaryOp(const sptr<lexer::Token>& op,
                          const std::string& operand) override;

  /// \copydoc IEmitter::emitBinaryOp
  std::string emitBinaryOp(const std::string& lhs, const sptr<lexer::Token>& op,
                           const std::string& rhs) override;

  /// \copydoc IEmitter::emitArrayAccess
  std::string emitArrayAccess(const std::string& arr,
                              const std::string& idx) override;

  /// \copydoc IEmitter::emitTemp
  std::string emitTemp(int number) override;

  /*-------------------------------------------------------------------------*/

  /* Statements */
  std::string code;

  /// \copydoc IEmitter::emitIf
  void emitIf(const std::string& cond,
              const std::function<void()>& thenBlock) override;

  /// \copydoc IEmitter::emitIfElse
  void emitIfElse(const std::string& cond,
                  const std::function<void()>& thenBlock,
                  const std::function<void()>& elseBlock) override;

  /// \copydoc IEmitter::emitWhile
  void emitWhile(const std::function<std::string()>& condGen,
                 const std::function<void()>& bodyGen) override;

  /// \copydoc IEmitter::emitDoWhile
  void emitDoWhile(const std::function<void()>& bodyGen,
                   const std::function<std::string()>& condGen) override;

  /// \copydoc IEmitter::emitBreak
  void emitBreak() override;

  /// \copydoc IEmitter::emitAssign
  void emitAssign(const std::string& target, const std::string& value) override;

  /// \copydoc IEmitter::emitArrayAssign
  void emitArrayAssign(const std::string& arr, const std::string& idx,
                       const std::string& value) override;

  /*-------------------------------------------------------------------------*/
};

// TODO AsmEmitter
// TODO IRBuilderEmitter (LLVM IR)
}  // namespace emit
