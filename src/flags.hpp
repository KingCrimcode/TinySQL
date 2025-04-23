#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Tinysql::Flags {
  struct Flags {
    bool help    = false;
    bool version = false;
    std::vector<std::string> unknownFlags;
  };

  enum Status : std::uint8_t {
    CONTINUE,
    EXIT_ERR,
    EXIT_OK
  };

  Flags parseFlags(const std::vector<std::string>& args);
  Status handleFlags(const Flags& flags);
}
