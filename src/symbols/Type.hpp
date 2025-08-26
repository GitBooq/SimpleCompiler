/**
 * @file Type.hpp
 * @brief Base type descriptor.
 */
#pragma once
#include <string>

#include "sptr.h"

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
  explicit Type(const std::string& n, int w) : name(n), width(w) {}
  virtual ~Type() = default;

  bool isNumeric() const {
    return name == "char" || name == "int" || name == "float";
  }

  inline static auto Bool = std::make_shared<Type>("bool", 1);
  inline static auto Char = std::make_shared<Type>("char", 1);
  inline static auto Int = std::make_shared<Type>("int", 4);
  inline static auto Float = std::make_shared<Type>("float", 8);

  /**
   * @brief Type conversion
   *
   * Compares two passed types and returns the "largest"
   * according to the hierarchy of numeric types:
   * - If at least one operand is 'float' - the result is 'float'.
   * - Otherwise, if at least one is 'int' - result is 'int'.
   * - Otherwise (both 'char') - result is 'char'.
   *
   * If at least one of the types is not numeric, returns 'nullptr'.
   *
   * @param t1 Type of first operand.
   * @param t2 Type of second operand.
   * @return Result type or nullptr.
   */
  static sptr<Type> max(const sptr<Type>& t1, const sptr<Type>& t2) {
    if (!t1->isNumeric() || !t2->isNumeric()) return nullptr;
    if (t1 == Float || t2 == Float) return Float;
    if (t1 == Int || t2 == Int) return Int;
    return Char;
  }
};

}  // namespace symbols