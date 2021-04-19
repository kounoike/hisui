#include "datetime.hpp"

#include <fmt/core.h>

#include <ctime>
#include <string>

namespace hisui::datetime {

std::string get_current_utc_string() {
  std::time_t timer;
  std::tm gm;
  std::time(&timer);
  gmtime_r(&timer, &gm);
  return fmt::format("{:04}{:02}{:02}{:02}{:02}{:02}", gm.tm_year + 1900,
                     gm.tm_mon + 1, gm.tm_mday, gm.tm_hour, gm.tm_min,
                     gm.tm_sec);
}

}  // namespace hisui::datetime
