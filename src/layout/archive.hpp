#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <boost/json/value.hpp>

#include "layout/source.hpp"

namespace hisui::layout {

class Archive {
 public:
  Archive(const std::filesystem::path& t_path, const boost::json::value& jv);
  void dump() const;
  void prepare();
  const SourceParameters& getSourceParameters() const;

 private:
  std::filesystem::path m_path;

  std::filesystem::path m_file_path;
  std::string m_connection_id;
  std::uint64_t m_start_time;
  std::uint64_t m_stop_time;
};

std::shared_ptr<Archive> parse_archive(const std::string&);

struct FindFileResult {
  bool found;
  std::filesystem::path path;
  std::string message;
};

FindFileResult find_file(const std::string&);
}  // namespace hisui::layout

