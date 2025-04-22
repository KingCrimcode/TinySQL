#pragma once
#include <string>

namespace Tinysql::Utils {
  /*
   * @brief Remove trailing whitespaces from string
   * @param __str String to trim
   * @return The new, trimmed string
   */
  std::string trim(const std::string& str_vw);
}
