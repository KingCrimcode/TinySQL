#include <cstdint>

namespace Tinysql {
  enum Status : std::uint8_t {
    CONTINUE,
    EXIT_ERR,
    EXIT_OK
  };
}
