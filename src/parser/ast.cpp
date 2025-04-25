#include <algorithm>
#include <cstdio>
#include <memory>
#include <print>

#include "ast.hpp"

namespace AST {
  void IntegerLiteral::print() const {
    std::print(stdout, "{}", value);
  }

  void FloatLiteral::print() const {
    std::print(stdout, "{}", value);
  }

  void StringLiteral::print() const {
    std::print(stdout, "{}", value);
  }

  void Identifier::print() const {
    std::print(stdout, "{}", name);
  }

  void SelectExpr::print() const {
    expr->print();
  }

  void TableFactor::print() const {
    table->print();
  }

  void SelectStatement::print() const {
    std::print(stdout, "Selected columns: ");
    std::ranges::for_each(select_expr_list, [](const std::unique_ptr<SelectExpr>& expr) {
      expr->print();
      std::print(stdout, " ");
    });
    std::println(stdout);
    std::print(stdout, "Tables columns: ");
    std::ranges::for_each(table_references, [](const std::unique_ptr<TableReference>& expr) {
      expr->print();
      std::print(stdout, " ");
    });
    std::println(stdout);
  }
}
