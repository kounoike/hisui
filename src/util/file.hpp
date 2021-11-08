#pragma once

#include <filesystem>
#include <string>

namespace hisui::util {

struct FindFileResult {
  bool found;
  std::filesystem::path path;
  std::string message;
};

FindFileResult find_file(const std::string&);

}  // namespace hisui::util
