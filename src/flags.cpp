#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <print>
#include <string>
#include <vector>

#include "flags.hpp"

#ifndef TINYSQL_VERSION
#define TINYSQL_VERSION "unknown"
#endif

namespace {
  void printFlags() {
    std::println("Usage: tinysql [options]");
    std::println(R"(Options:
      -h, --help     -> Prints this menu 
      -v, --version  -> Prints the current version)");
  }
}

namespace Tinysql::Flags {
  Flags parseFlags(const std::vector<std::string>& args) {
    Flags flags;

    for (size_t i = 0; i < args.size(); ++i) {
      if (args[i] == "-h" || args[i] == "--help") {
        flags.help = true;
      } else if (args[i] == "-v" || args[i] == "--version") {
        flags.version = true;
      } else {
        flags.unknownFlags.push_back(args[i]);
      }
    }

    return flags;
  }

  Status handleFlags(const Flags& flags) {
    if (!flags.unknownFlags.empty()) {
      std::print(stdout, "Error: unknown options: ");
      std::ranges::for_each(flags.unknownFlags,
                            [](const std::string& arg) { std::print(stdout, "{}", arg); });
      std::println(stdout, "\nUse -h or --help for a list of options");
      return Status::EXIT_ERR;
    }
    if (flags.help) {
      printFlags();
      return Status::EXIT_OK;
    }
    if (flags.version) {
      std::println(stdout, "TinySQL version {}", TINYSQL_VERSION);
      return Status::EXIT_OK;
    }
    return Status::CONTINUE;
  }
}
