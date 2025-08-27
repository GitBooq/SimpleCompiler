/**/

#include "Stmt.h"

#include "Expr.hpp"
#include "IEmitter.hpp"

namespace ast {

void Seq::emit(emit::IEmitter& out) const {
  if (first) first->emit(out);
  if (second) second->emit(out);
}

void If::emit(emit::IEmitter& out) const {
  // Get string/value of condition(expr) using Expr::emit
  std::string condVal = condition->emit(out);

  // Ask emitter to generate if w/o else
  out.emitIf(condVal, [this, &out]() {
    // then body
    thenStmt->emit(out);
  });
}

void Else::emit(emit::IEmitter& out) const {
  auto condVal = condition->emit(out);
  out.emitIfElse(
      condVal, [this, &out]() { thenStmt->emit(out); },
      [this, &out]() { elseStmt->emit(out); });
}

void While::emit(emit::IEmitter& out) const {
  out.emitWhile([this, &out]() { return condition->emit(out); },
                [this, &out]() { body->emit(out); });
}

void Do::emit(emit::IEmitter& out) const {
  out.emitDoWhile([this, &out]() { body->emit(out); },
                  [this, &out]() { return condition->emit(out); });
}

void Break::emit(emit::IEmitter& out) const { out.emitBreak(); }

void Set::emit(emit::IEmitter& out) const {
  std::string lhsVal = id->emit(out);
  std::string rhsVal = expr->emit(out);
  out.emitAssign(lhsVal, rhsVal);
}

void SetElem::emit(emit::IEmitter& out) const {
  // arrayAccess and expr - ar Expr, emit them first:
  auto arrStr = arrayAccess->emit(out);
  auto valStr = expr->emit(out);
  out.emitArrayAssign(
      arrStr,
      /* The index from Access can be extracted directly in emitArrayAccess,
         but if arrayAccess is a concrete Access node,
         its emit() will already return arr[idx] as a string */
      "",  // or idx, if the method is implemented separately
      valStr);
}

}  // namespace ast