#include "stringUtils.hpp"
#include <string>

namespace Tinysql::Utils {
  std::string trim(const std::string& str) {
    const auto first = str.find_first_not_of(" \t");
    const auto last  = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
  }
}
