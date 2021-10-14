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
#include "layout/region.hpp"

namespace hisui::layout {

enum struct ContainerFormat {
  WebM,
  MP4,
};

class Metadata {
 public:
  Metadata(const std::string& file_path, const boost::json::value& jv);
  void Dump() const;

 private:
  std::filesystem::path m_path;

  std::vector<std::string> m_audio_source_filenames;
  std::uint64_t m_bitrate;
  ContainerFormat m_format;
  std::uint64_t m_width;
  std::uint64_t m_height;
  bool m_trim;

  std::vector<std::shared_ptr<Region>> m_regions;
};

Metadata parse_metadata(const std::string&);

}  // namespace hisui::layout
