#include <cstdio>
#include <string>
#include <vector>

#include "flags.hpp"
#include "repl.hpp"

using namespace Tinysql::Flags;

extern int yyparse();

int main(int argc, char** argv) {
  // Handle launch flags
  const std::vector<std::string> args{argv + 1, argc + argv};
  const Flags flags   = parseFlags(args);
  const Tinysql::Status status = handleFlags(flags);
  switch (status) {
    case Tinysql::Status::CONTINUE: break;
    case Tinysql::Status::EXIT_OK: return 0;
    case Tinysql::Status::EXIT_ERR: return 1;
  }

  Tinysql::REPL::run();

  return 0;
}
