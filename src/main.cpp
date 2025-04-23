#include <cstdio>
#include <iostream>
#include <print>
#include <string>
#include <vector>

#include "flags.hpp"
#include "lexer.hpp"
#include "stringUtils.hpp"

using namespace Tinysql::Utils;
using namespace Tinysql::Flags;

extern int yyparse();

namespace {
  void handleInMemoryQuery(const std::string& input) {
    // Create buffer for Flex based on string
    YY_BUFFER_STATE buffer = yy_scan_string(input.c_str());
    // Call Bison
    yyparse();
    // Free the buffer space
    yy_delete_buffer(buffer);
  }
}

int main(int argc, char** argv) {
  // Handle launch flags
  const std::vector<std::string> args{argv + 1, argc + argv};
  const Flags flags   = parseFlags(args);
  const Status status = handleFlags(flags);
  switch (status) {
    case Status::CONTINUE: break;
    case Status::EXIT_OK: return 0;
    case Status::EXIT_ERR: return 1;
  }

  // One line may contain more than one query, they will be stored and executed only after
  // semicolon endline
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

    // Continue reading if line does not end in a semicolon
    if (line.find(';') == std::string::npos || line.back() != ';') {
      queries += " ";
      continue;
    }

    // Extract every user query excluding semicolons, trim the whitespaces and send to the parser
    while (!queries.empty()) {
      auto first_semicolon = queries.find_first_of(';');

      current_query = queries.substr(0, first_semicolon);
      current_query = trim(current_query);
      handleInMemoryQuery(current_query);

      queries = queries.substr(queries.find_first_of(';') + 1);
    }
    queries.clear();
  }

  return 0;
}
