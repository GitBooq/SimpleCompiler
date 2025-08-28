/**
 * @file Expr.hpp
 * @brief Base class for all expression nodes in the Abstract Syntax Tree (AST).
 */

#pragma once
#include "ASTNode.hpp"
#include "Id.hpp"
#include "sptr.h"

/* Forward declarations */
namespace symbols {
struct Type;
}

namespace lexer {
struct Token;
struct Word;
}  // namespace lexer

namespace emit {
struct IEmitter;
}
//-----------------------//

namespace ast {
/**
 * @brief Base class for all expressions AST nodes.
 *
 * Stores source location and semantic type.
 */
struct Expr : public ASTNode {
  sptr<symbols::Type> exprType;

  explicit Expr(SourceLocation loc) : ASTNode(loc), exprType(nullptr) {}

  virtual ~Expr() = default;

  /**
   * @brief Generate code for expression via abstract emitter interface.
   * @param out The emitter interface, which decides how and where to generate
   * code.
   * @return The name of a temporary variable or other identifier where the
   * result is stored.
   *
   * AST does not manage names and formats — this is done by the emitter.
   */
  virtual std::string emit(emit::IEmitter& out) const = 0;
};

/**
 * @brief Base for all binary operations
 */
struct Op : public Expr {
  sptr<Expr> lhs;
  sptr<Expr> rhs;
  sptr<lexer::Token> op_tok;

  Op(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> l, sptr<Expr> r)
      : Expr(loc), lhs(std::move(l)), rhs(std::move(r)), op_tok(tok) {}

  std::string emit(emit::IEmitter& out) const override;
};

/**
 * @brief Arithmetic operations (+, -, *, /)
 */
struct Arith : public Op {
  Arith(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> l, sptr<Expr> r);
};

/**
 * @brief Unary operations (-x, !x, etc.)
 */
struct Unary : public Expr {
  sptr<Expr> expr;
  sptr<lexer::Token> op_tok;

  Unary(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> e)
      : Expr(loc), expr(std::move(e)), op_tok(tok) {}

  std::string emit(emit::IEmitter& out) const override;
};

/**
 * @brief Relational operations (<, >, <=, >=, ==, !=)
 */
struct Rel : public Op {
  Rel(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> l, sptr<Expr> r);
};

struct Equal : public Rel {
  using Rel::Rel;
};
struct NotEqual : public Rel {
  using Rel::Rel;
};
struct Less : public Rel {
  using Rel::Rel;
};
struct Greater : public Rel {
  using Rel::Rel;
};
struct LessEqual : public Rel {
  using Rel::Rel;
};
struct GreaterEqual : public Rel {
  using Rel::Rel;
};

/**
 * @brief Logical binary operations (&&, ||)
 */
struct Logical : public Op {
  Logical(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> l,
          sptr<Expr> r);
};
struct And : public Logical {
  using Logical::Logical;
};
struct Or : public Logical {
  using Logical::Logical;
};

/**
 * @brief Logical unary NOT (!x)
 */
struct Not : public Unary {
  Not(SourceLocation loc, sptr<lexer::Token> tok, sptr<Expr> e);
};

/**
 * @brief Literal constants (number, true, false, etc.)
 */
struct Constant : public Expr {
  sptr<lexer::Word> value;
  explicit Constant(SourceLocation loc, sptr<lexer::Word> v);
  std::string emit(emit::IEmitter& out) const override;
};

/**
 * @brief Temporary variable (t1, t2, etc.)
 */
struct Temp : public Expr {
  int number;
  Temp(SourceLocation loc, int n, sptr<symbols::Type> t);
  std::string emit(emit::IEmitter& out) const override;
};

/**
 * @brief Access to array element (a[i])
 */
struct Access : public Expr {
  sptr<Expr> array;
  sptr<Expr> index;
  Access(SourceLocation loc, sptr<Expr> arr, sptr<Expr> idx);
  std::string emit(emit::IEmitter& out) const override;
};

/**
 * @brief Identificator node
 */
struct IdExpr : public Expr {
  sptr<symbols::Id> sym;
  IdExpr(SourceLocation loc, sptr<symbols::Id> s)
      : Expr(loc), sym(std::move(s)) {
    exprType = sym->type;
  }
  std::string emit(emit::IEmitter& out) const override;
};
}  // namespace ast