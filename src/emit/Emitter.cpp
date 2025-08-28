/*
File: Emitter.cpp
Emitter implementation.
*/
#include "Emitter.h"

#include "Token.hpp"

namespace emit {
/* Expressions */
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

std::string TextEmitter::emitIdentifier(const std::string& name,
                                        int /*offset*/) {
  return name;
}
/*-------------------------------------------------------------------------*/

/* Statements */
void TextEmitter::emitIf(const std::string& cond,
                         const std::function<void()>& thenBlock) {
  code += "if (" + cond + ") {\n";
  thenBlock();
  code += "}\n";
}

void TextEmitter::emitIfElse(const std::string& cond,
                             const std::function<void()>& thenBlock,
                             const std::function<void()>& elseBlock) {
  code += "if (" + cond + ") {\n";
  thenBlock();
  code += "} else {\n";
  elseBlock();
  code += "}\n";
}

void TextEmitter::emitWhile(const std::function<std::string()>& condGen,
                            const std::function<void()>& bodyGen) {
  code += "while (" + condGen() + ") {\n";
  bodyGen();
  code += "}\n";
}

void TextEmitter::emitDoWhile(const std::function<void()>& bodyGen,
                              const std::function<std::string()>& condGen) {
  code += "do {\n";
  bodyGen();
  code += "} while (" + condGen() + ");\n";
}

void TextEmitter::emitBreak() { code += "break;\n"; }

void TextEmitter::emitAssign(const std::string& target,
                             const std::string& value) {
  code += target + " = " + value + ";\n";
}

void TextEmitter::emitArrayAssign(const std::string& arr,
                                  const std::string& idx,
                                  const std::string& value) {
  code += arr + "[" + idx + "] = " + value + ";\n";
}
/*-------------------------------------------------------------------------*/
}  // namespace emit