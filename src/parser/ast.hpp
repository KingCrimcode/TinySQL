#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace AST {
  // Root
  class ASTNode {
  public:
    virtual void print() const = 0;
    virtual ~ASTNode() = default;
  };

  // Expressions
  class Expr : public ASTNode {
  public:
    ~Expr() override = default;
  };

  class LiteralExpr : public Expr {};

  class IntegerLiteral : public LiteralExpr {
  public:
    int value;
    explicit IntegerLiteral(int value) :
        value(value) {}
    void print() const override;
  };

  class FloatLiteral : public LiteralExpr {
  public:
    double value;
    explicit FloatLiteral(double value) :
        value(value) {}
    void print() const override;
  };

  class StringLiteral : public LiteralExpr {
  public:
    std::string value;
    explicit StringLiteral(std::string value) :
        value(std::move(value)) {}
    void print() const override;
  };

  class Identifier : public Expr {
  public:
    std::string name;
    explicit Identifier(std::string name) :
        name(std::move(name)) {}
    void print() const override;
  };

  // Select Expression
  class SelectExpr : public ASTNode {
  public:
    std::unique_ptr<Expr> expr;
    // here will come an optional for alias, if I implement it
    SelectExpr(std::unique_ptr<Expr> expr) :
        expr(std::move(expr)) {}
    void print() const override;
  };

  // Table Reference
  class TableReference : public ASTNode {};

  class TableFactor : public TableReference {
  public:
    std::unique_ptr<Identifier> table;
    explicit TableFactor(std::unique_ptr<Identifier> name) :
        table(std::move(name)) {}
    void print() const override;
  };

  // Statements
  class Statement : public ASTNode {};

  class SelectStatement : public Statement {
  public:
    std::vector<std::unique_ptr<SelectExpr>> select_expr_list;
    std::vector<std::unique_ptr<TableReference>> table_references;
    SelectStatement(std::vector<std::unique_ptr<SelectExpr>> select_expr_list,
                    std::vector<std::unique_ptr<TableReference>> table_references) :
        select_expr_list(std::move(select_expr_list)),
        table_references(std::move(table_references)) {}
    void print() const override;
  };
}
