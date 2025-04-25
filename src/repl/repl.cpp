#include <functional>
#include <iostream>
#include <print>
#include <string>
#include <unordered_map>

#include "ast.hpp"
#include "enums.hpp"
#include "lexer.hpp"
#include "stringUtils.hpp"

#include "repl.hpp"

using namespace Tinysql::Utils;

extern AST::ASTNode* astRoot;
extern int yyparse();

namespace {
  void printOptions() {
    std::print(stdout,
               ".exit  Exit this program\n"
               ".help  Show help text\n");
  }
  const std::unordered_map<std::string, std::function<Tinysql::Status(const std::string& args)>>
      command_map = {{".exit",
                      [](const std::string&) {
                        std::println();
                        return Tinysql::Status::EXIT_OK;
                      }},
                     {".help", [](const std::string&) {
                        printOptions();
                        return Tinysql::Status::CONTINUE;
                      }}};

  Tinysql::Status handleCommand(const std::string& input) {
    std::string command;
    std::string args;
    auto sep_index = input.find(' ');
    if (sep_index != std::string::npos) {
      command = input.substr(0, sep_index);
      args    = input.substr(sep_index + 1);
    } else {
      command = input;
    }

    auto iter = command_map.find(command);
    if (iter != command_map.end()) {
      return iter->second(args);
    }
    std::println(stdout, "Unknown command {}", command);
    return Tinysql::Status::CONTINUE;
  }

  void handleQuery(const std::string& input) {
    // Create buffer for Flex based on string
    YY_BUFFER_STATE buffer = yy_scan_string(input.c_str());
    // Call Bison
    yyparse();
    // Free the buffer space
    yy_delete_buffer(buffer);

    astRoot->print();
  }
}

namespace Tinysql::REPL {
  void run() {
    std::println(stdout, "Enter '.help' for usage hints");
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

      if (queries.empty() && line.front() == '.') {
        const Tinysql::Status status = handleCommand(line);
        if (status == EXIT_OK) {
          return;
        }
        continue;
      }

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
        handleQuery(current_query);

        queries = queries.substr(queries.find_first_of(';') + 1);
      }
      queries.clear();
    }
  }
}
