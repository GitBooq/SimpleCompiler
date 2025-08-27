/**
 * @file Stmt.hpp
 * @brief Statement nodes for the Abstract Syntax Tree (AST).
 */
#pragma once

#include "ASTNode.hpp"
#include "sptr.h"

// Forward declarations
namespace ast {
struct Expr;
}

namespace emit {
struct IEmitter;
}
//-----------------------//

namespace ast {
/**
 * @brief Base class for all operators (statement).
 */
struct Stmt : public ASTNode {
  explicit Stmt(SourceLocation loc) : ASTNode(loc) {}
  virtual ~Stmt() = default;

  /// Codegen for statement
  virtual void emit(emit::IEmitter& out) const = 0;
};

/**
 * @brief Sequencing (stmt1; stmt2;)
 */
struct Seq : public Stmt {
  sptr<Stmt> first;
  sptr<Stmt> second;

  Seq(SourceLocation loc, sptr<Stmt> s1, sptr<Stmt> s2)
      : Stmt(loc), first(std::move(s1)), second(std::move(s2)) {}
  void emit(emit::IEmitter& out) const override;
};

/**
 * @brief if (cond) stmt;
 */
struct If : public Stmt {
  sptr<Expr> condition;
  sptr<Stmt> thenStmt;

  If(SourceLocation loc, sptr<Expr> cond, sptr<Stmt> thenBranch)
      : Stmt(loc),
        condition(std::move(cond)),
        thenStmt(std::move(thenBranch)) {}
  void emit(emit::IEmitter& out) const override;
};

/**
 * @brief if (cond) stmt; else stmt;
 */
struct Else : public Stmt {
  sptr<Expr> condition;
  sptr<Stmt> thenStmt;
  sptr<Stmt> elseStmt;

  Else(SourceLocation loc, sptr<Expr> cond, sptr<Stmt> thenBranch,
       sptr<Stmt> elseBranch)
      : Stmt(loc),
        condition(std::move(cond)),
        thenStmt(std::move(thenBranch)),
        elseStmt(std::move(elseBranch)) {}
  void emit(emit::IEmitter& out) const override;
};

/**
 * @brief while (cond) stmt;
 */
struct While : public Stmt {
  sptr<Expr> condition;
  sptr<Stmt> body;

  While(SourceLocation loc, sptr<Expr> cond, sptr<Stmt> bodyStmt)
      : Stmt(loc), condition(std::move(cond)), body(std::move(bodyStmt)) {}
  void emit(emit::IEmitter& out) const override;
};

/**
 * @brief do { body } while (cond);
 */
struct Do : public Stmt {
  sptr<Stmt> body;
  sptr<Expr> condition;

  Do(SourceLocation loc, sptr<Stmt> bodyStmt, sptr<Expr> cond)
      : Stmt(loc), body(std::move(bodyStmt)), condition(std::move(cond)) {}
  void emit(emit::IEmitter& out) const override;
};

/**
 * @brief break;
 */
struct Break : public Stmt {
  Break(SourceLocation loc) : Stmt(loc) {}
  void emit(emit::IEmitter& out) const override;
};

/**
 * @brief x = expr;
 */
struct Set : public Stmt {
  sptr<Expr> id;    // variable
  sptr<Expr> expr;  // right expression

  Set(SourceLocation loc, sptr<Expr> identifier, sptr<Expr> value)
      : Stmt(loc), id(std::move(identifier)), expr(std::move(value)) {}
  void emit(emit::IEmitter& out) const override;
};

/**
 * @brief arr[index] = expr;
 */
struct SetElem : public Stmt {
  sptr<Expr> arrayAccess;  // Access ( arr[index] )
  sptr<Expr> expr;         // right expression

  SetElem(SourceLocation loc, sptr<Expr> access, sptr<Expr> value)
      : Stmt(loc), arrayAccess(std::move(access)), expr(std::move(value)) {}
  void emit(emit::IEmitter& out) const override;
};

}  // namespace ast
