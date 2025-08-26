/**
 * @file ASTNode.hpp
 * @brief Base for all nodes in the Abstract Syntax Tree (AST).
 */

#pragma once
#include <ostream>
#include <string>

/**
 * @brief Location of node in source text.
 */
struct SourceLocation {
  int line;   /**< Line number */
  int column; /**< Column number */
};

/**
 * @brief Base class for all AST nodes.
 */
struct ASTNode {
  SourceLocation location; /**< Position in the source text */

  ASTNode(SourceLocation loc) : location(loc) {}

  virtual ~ASTNode() = default;
};
