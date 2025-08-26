/**
 * @file Tag.hpp
 * @brief Enumeration of token tags for the lexer.
 */
#pragma once

namespace lexer {

/**
 * @enum Tag
 * @brief Symbolic codes for tokens
 * characters.
 */
enum class Tag : int {
  ID,     /**< Identifier */
  NUM,    /**< Integer literal */
  REAL,   /**< Floating-point literal */
  TRUE_,  /**< Boolean literal true */
  FALSE_, /**< Boolean literal false */
  IF,     /**< Keyword if */
  ELSE,   /**< Keyword else */
  WHILE,  /**< Keyword while */
  DO,     /**< Keyword do */
  BREAK,  /**< Keyword break */
  BASIC,  /**< Basic type (int, float, bool) */
  OR,     /**< Logical operator || */
  AND,    /**< Logical operator && */
  EQ,     /**< Comparison operator == */
  NE,     /**< Comparison operator != */
  LE,     /**< Comparison operator <= */
  GE,     /**< Comparison operator >= */
  INDEX,  /**< Array indexing */
  MINUS,  /**< Unary minus */
  TEMP,   /**< Temporary variable */

  OP_PLUS,  /**< Bin operator '+' */
  OP_MINUS, /**< Bin operator '-' */
  OP_DIV,   /**< Bin operator '/' */
  OP_MUL,   /**< Bin operator '*' */

  bitAND,   /**< Bit operator '&' */
  bitOR,    /**< Bit operator '|' */
  ASSIGN,   /**< Assign operator '=' */
  UnaryNOT, /**< Bit not operator '!' */
  LESS,     /**< Relation less operator '<' */
  GREATER   /**< Relation greater operator '>' */
};
}  // namespace lexer
