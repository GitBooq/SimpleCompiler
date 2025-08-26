/*
File: Emitter.cpp
Emitter implementation.
*/
#include "Emitter.h"

#include "Token.hpp"

namespace emit {
std::string TextEmitter::emitLoadConst(const sptr<lexer::Token>& value) {
  return value->lexeme;
}

std::string TextEmitter::emitUnaryOp(const sptr<lexer::Token>& op,
                                     const std::string& operand) {
  return op->lexeme + operand;
}

std::string TextEmitter::emitBinaryOp(const std::string& lhs,
                                      const sptr<lexer::Token>& op,
                                      const std::string& rhs) {
  return lhs + " " + op->lexeme + " " + rhs;
}

std::string TextEmitter::emitArrayAccess(const std::string& arr,
                                         const std::string& idx) {
  return arr + "[" + idx + "]";
}

std::string TextEmitter::emitTemp(int number) {
  return "t" + std::to_string(number);
}
}  // namespace emit