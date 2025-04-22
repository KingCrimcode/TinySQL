#include <cstdio>
#include <iostream>
#include <print>
#include <string>
#include <vector>

#include "stringUtils.hpp"
#include "lexer.hpp"

#ifndef TINYSQL_VERSION
#define TINYSQL_VERSION "unknown"
#endif

using namespace Tinysql::Utils;

extern int yyparse();

namespace {
  // Print help flag
  void help() {
    std::println("Usage: tinysql [options]");
    std::println(R"(Options:
      -h, --help     -> Prints this menu 
      -v, --version  -> Prints the current version)");
  }
}

void handleQuery(const std::string& input) {
  YY_BUFFER_STATE buffer = yy_scan_string(input.c_str());
  yyparse();
  yy_delete_buffer(buffer);
}

int main(int argc, char** argv) {
  // Handle launch flags
  const std::vector<std::string> args{argv + 1, argc + argv};
  for (const auto& arg : args) {
    if (arg == "-h" || arg == "--help") {
      help();
      return 0;
    }
    if (arg == "-v" || arg == "--version") {
      std::println("{}", TINYSQL_VERSION);
      return 0;
    }
    std::println(stderr, "[ERROR] Unknown option '{}' !", arg);
    help();
    return 1;
  }

  std::string line;
  std::string queries;
  std::string current_query;
  while (true) {
    // Alter print for multiline statements
    if (queries.empty()) {
      std::print(stdout, "tinysql> ");
    } else {
      std::print(stdout, "    ...> ");
    }

    std::getline(std::cin, line);
    line = trim(line);
    queries += line;

    if (line.find(';') == std::string::npos || line.back() != ';') {
      queries += " ";
      continue;
    }

    while (!queries.empty()) {
      auto first_semicolon = queries.find_first_of(';');

      current_query = queries.substr(0, first_semicolon);
      current_query = trim(current_query);
      handleQuery(current_query);

      queries = queries.substr(queries.find_first_of(';') + 1);
    }
    queries.clear();
  }

  return 0;
}
