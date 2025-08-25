/**
 * @file Env.hpp
 * @brief Environment (symbol table) for a scope.
 */
#pragma once
#include <memory>
#include <unordered_map>

#include "Id.hpp"
#include "sptr.h"

namespace symbols {

/**
 * @brief Environment (symbol table) for a scope.
 *
 * Maps names to Id objects and supports nested scopes
 * via a pointer to the parent environment.
 */
struct Env {
  /** Map of identifier names to their descriptors. */
  std::unordered_map<std::string, sptr<Id>> table;

  /** Parent (outer) environment, or nullptr if global scope. */
  sptr<Env> prev;

  /**
   * @brief Construct a new Env.
   * @param p Parent environment (outer scope).
   */
  Env(sptr<Env> p = nullptr) : prev(std::move(p)) {}

  /**
   * @brief Insert a new identifier into the current scope.
   * @param name Symbol name.
   * @param id Descriptor of the symbol.
   */
  void put(const std::string& name, sptr<Id> id) {
    table[name] = std::move(id);
  }

  /**
   * @brief Look up an identifier in the current and outer scopes.
   * @param name Symbol name to find.
   * @return std::shared_ptr<Id> Descriptor if found, otherwise nullptr.
   */
  sptr<Id> get(const std::string& name) const {
    for (auto env = this; env; env = env->prev.get()) {
      auto it = env->table.find(name);
      if (it != env->table.end()) return it->second;
    }
    return nullptr;
  }
};

}  // namespace symbols