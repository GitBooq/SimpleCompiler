/**
 * @file Type.hpp
 * @brief Base type descriptor.
 */
#pragma once
#include <string>

namespace symbols {
/**
 * @brief Base type descriptor.
 *
 * Represents a primitive or user-defined type.
 * Stores the type name and its width in bytes.
 */
struct Type {
  /** Name of the type (e.g., "int", "float", "bool"). */
  std::string name;
  /** Width of the type in bytes. */
  int width;

  /**
   * @brief Construct a new Type.
   * @param n Type name.
   * @param w Width in bytes.
   */
  Type(const std::string& n, int w) : name(n), width(w) {}
  virtual ~Type() = default;
};

}  // namespace symbols