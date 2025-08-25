/**
 * @file Id.hpp
 * @brief Identifier descriptor.
 */
#pragma once
#include <memory>
#include <string>

#include "Type.hpp"
#include "sptr.h"

namespace symbols {

/**
 * @brief Identifier descriptor.
 *
 * Represents a variable or symbol: its name, type, and memory offset.
 */
struct Id {
  /** Name of the identifier. */
  std::string name;

  /** Type of the identifier. */
  sptr<Type> type;

  /** Offset in bytes relative to its scope's base address. */
  int offset;

  /**
   * @brief Construct a new Id.
   * @param n Identifier name.
   * @param t Shared pointer to the type.
   * @param off Memory offset in bytes.
   */
  Id(std::string n, sptr<Type> t, int off)
      : name(std::move(n)), type(std::move(t)), offset(off) {}
};

}  // namespace symbols