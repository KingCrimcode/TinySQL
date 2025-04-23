#pragma once

#include <string>
#include <vector>

#include "enums.hpp"

namespace Tinysql::Flags {
  struct Flags {
    bool help    = false;
    bool version = false;
    std::vector<std::string> unknownFlags;
  };

  Flags parseFlags(const std::vector<std::string>& args);
  Status handleFlags(const Flags& flags);
}
