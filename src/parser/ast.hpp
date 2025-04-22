#pragma once

#include <string>
#include <utility>
#include <vector>

struct SelectQuery {
  std::vector<std::string> columns;
  std::string table;

  SelectQuery(std::vector<std::string> cols, std::string tbl) :
      columns(std::move(cols)), table(std::move(tbl)) {}
};
