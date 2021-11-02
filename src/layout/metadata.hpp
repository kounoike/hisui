#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <boost/json/array.hpp>
#include <boost/json/impl/array.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/string.hpp>
#include <boost/json/system_error.hpp>
#include <boost/json/value.hpp>

#include "config.hpp"
#include "layout/archive.hpp"
#include "layout/cell_util.hpp"
#include "layout/region.hpp"

namespace hisui::layout {

class Metadata {
 public:
  Metadata(const std::string& file_path, const boost::json::value& jv);
  void dump() const;
  void prepare();
  void copyToConfig(hisui::Config*) const;
  double getMaxEndTime() const;
  std::vector<std::shared_ptr<Region>> getRegions() const;
  Resolution getResolution() const;
  void resetPath() const;

  std::vector<hisui::Archive> getAudioArchives() const;
  double getMaxStopTimeOffset() const;

 private:
  std::filesystem::path m_path;

  std::vector<std::string> m_audio_source_filenames;
  std::uint64_t m_bitrate;
  hisui::config::OutContainer m_format;
  Resolution m_resolution;
  bool m_trim;
  std::filesystem::path m_working_path;

  std::vector<std::shared_ptr<Archive>> m_audio_archives;
  double m_audio_max_end_time;
  double m_max_end_time;
  std::vector<std::shared_ptr<Region>> m_regions;

  void parseVideoLayout(boost::json::object j);
  std::shared_ptr<Region> parseRegion(const std::string& name,
                                      boost::json::object jo);
};

Metadata parse_metadata(const std::string&);

}  // namespace hisui::layout
