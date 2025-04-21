#include <print>
#include <string>
#include <vector>

#ifndef TINYSQL_VERSION
#define TINYSQL_VERSION "unknown"
#endif

extern int yyparse();

namespace {
  void help() {
    std::println("Usage: tinysql [options]");
    std::println(R"(Options:
      -h, --help     -> Prints this menu 
      -v, --version  -> Prints the current version)");
  }
}

int main(int argc, char** argv) {
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

  yyparse();
}
