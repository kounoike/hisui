#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <boost/json/value.hpp>

#include <archive.hpp>
#include "layout/interval.hpp"
#include "layout/source.hpp"

namespace hisui::layout {

class Archive {
 public:
  Archive(const std::filesystem::path& t_path, const boost::json::value& jv);
  void dump() const;
  void prepare();
  const SourceParameters getSourceParameters(const std::size_t) const;
  void substructTrimIntervals(const TrimIntervals&);
  Interval getInterval() const;
  hisui::Archive getArchive() const;

 private:
  std::filesystem::path m_path;

  std::filesystem::path m_file_path;
  std::string m_connection_id;
  double m_start_time;
  double m_stop_time;
};

std::shared_ptr<Archive> parse_archive(const std::string&);

}  // namespace hisui::layout
