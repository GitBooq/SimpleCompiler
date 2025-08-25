/**
 * @file Array.hpp
 * @brief Array type descriptor.
 */
#pragma once
#include <memory>

#include "Type.hpp"
#include "sptr.h"

namespace symbols {

/**
 * @brief Array type descriptor.
 *
 * Derived from Type, represents an array of a given element type.
 */
struct Array : public Type {
  /** Element type of the array. */
  sptr<Type> of;
  /** Number of elements in the array. */
  int size;

  /**
   * @brief Construct a new Array type.
   * @param sz Number of elements.
   * @param elemType Shared pointer to the element type.
   */
  Array(int sz, sptr<Type> elemType)
      : Type(elemType->name + "[]", sz * elemType->width),
        of(std::move(elemType)),
        size(sz) {}
};

}  // namespace symbols