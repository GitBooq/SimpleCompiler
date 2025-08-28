// TypeToken.hpp
#pragma once

#include "Type.hpp"
#include "Word.hpp"
#include "sptr.h"

namespace lexer {
/**
 * @brief Token for basic types (int, float, bool, char).
 *
 * Inherited from Word so that it can be stored in the words table.
 * Additionally contains a pointer to the semantic description of the type.
 */
struct TypeToken : public Word {
  /// Pointer to an obj type in symbols module
  sptr<symbols::Type> typeInfo;

  TypeToken(const sptr<symbols::Type>& t, SourceLocation loc = {})
      : Word(t->name, Tag::BASIC, loc), typeInfo(t) {}
};
}  // namespace lexer