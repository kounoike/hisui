#include "util/file.hpp"

#include <fmt/core.h>

#include <filesystem>
#include <string>

namespace hisui::util {

FindFileResult find_file(const std::string& filename) {
  auto path = std::filesystem::path(filename);
  if (path.is_absolute()) {
    if (!std::filesystem::exists(path)) {
      return {.found = false,
              .message = fmt::format("does not exist path({})", filename)};
    }
    return {.found = true, .path = path};
  }
  if (std::filesystem::exists(path)) {
    return {.found = true, .path = std::filesystem::absolute(path)};
  }
  path = std::filesystem::absolute(path.filename());
  if (std::filesystem::exists(path)) {
    return {.found = true, .path = path};
  }
  return {.found = false,
          .message = fmt::format("does not exist path({})", filename)};
}

}  // namespace hisui::util
