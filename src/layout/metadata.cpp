#include "layout/metadata.hpp"

#include <spdlog/spdlog.h>

#include <fstream>
#include <regex>
#include <stdexcept>

#include <boost/json/array.hpp>
#include <boost/json/impl/array.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/string.hpp>
#include <boost/json/system_error.hpp>
#include <boost/json/value.hpp>
#include <boost/json/value_to.hpp>

#include "layout/archive.hpp"
#include "util/json.hpp"

namespace hisui::layout {

void Metadata::parseVideoLayout(boost::json::object j) {
  auto key = "video_layout";
  if (!j.contains(key) || j[key].is_null()) {
    return;
  }
  auto vl = j[key].as_object();
  for (const auto& region : vl) {
    std::string name(region.key());
    if (region.value().is_object()) {
      m_regions.push_back(parseRegion(name, region.value().as_object()));
    } else {
      throw std::invalid_argument(
          fmt::format("region: {} is not object", name));
    }
  }
}

void Metadata::dump() const {
  spdlog::debug("format: {}",
                m_format == ContainerFormat::MP4 ? "mp4" : "webm");
  spdlog::debug("bitrate: {}", m_bitrate);
  spdlog::debug("resolution: {}x{}", m_resolution.width, m_resolution.height);
  spdlog::debug("trim: {}", m_trim);
  spdlog::debug("audio_sources: [{}]",
                fmt::join(m_audio_source_filenames, ", "));
  spdlog::debug("video_layout");
  for (const auto& region : m_regions) {
    region->dump();
    spdlog::debug("");
  }
  for (const auto& a : m_audio_archives) {
    a->dump();
  }
}

Metadata::Metadata(const std::string& file_path, const boost::json::value& jv)
    : m_path(file_path) {
  if (m_path.is_relative()) {
    m_path = std::filesystem::absolute(m_path);
  }
  const auto current_path = std::filesystem::current_path();
  std::filesystem::current_path(m_path.parent_path());

  boost::json::object j;
  if (jv.is_object()) {
    j = jv.as_object();
  } else {
    throw std::runtime_error("jv is not object");
  }

  m_bitrate = static_cast<std::uint64_t>(
      hisui::util::get_double_from_json_object_with_default(j, "bitrate", 0));
  auto format = hisui::util::get_string_from_json_object_with_default(
      j, "format", "webm");
  if (format == "mp4") {
    m_format = ContainerFormat::MP4;
  } else if (format == "webm") {
    m_format = ContainerFormat::WebM;
  } else {
    throw std::invalid_argument(fmt::format("invalid format: {}", format));
  }

  std::string resolution(
      hisui::util::get_string_from_json_object(j, "resolution"));

  std::smatch m;
  if (std::regex_match(resolution, m, std::regex(R"((\d+)x(\d+))"))) {
    m_resolution.width = std::stoull(m[1].str());
    m_resolution.height = std::stoull(m[2].str());
  } else {
    throw std::invalid_argument(
        fmt::format("invalid resolution: {}", resolution));
  }
  m_trim = hisui::util::get_bool_from_json_object_with_default(j, "trim", true);

  auto audio_sources = hisui::util::get_array_from_json_object_with_default(
      j, "audio_sources", boost::json::array());

  for (const auto& v : audio_sources) {
    if (v.is_string()) {
      m_audio_source_filenames.push_back(std::string(v.as_string()));
    } else {
      throw std::invalid_argument(
          fmt::format("{} contains non-string values", "audio_sources"));
    }
  }
  // TODO(haruyama): audio_sources_excluded
  parseVideoLayout(j);
}

Metadata parse_metadata(const std::string& filename) {
  std::ifstream i(filename);
  if (!i.is_open()) {
    throw std::runtime_error(
        fmt::format("failed to open metadata json file: {}", filename));
  }
  std::string string_json((std::istreambuf_iterator<char>(i)),
                          std::istreambuf_iterator<char>());
  boost::json::error_code ec;
  boost::json::value jv = boost::json::parse(string_json, ec);
  if (ec) {
    throw std::runtime_error(fmt::format(
        "failed to parse metadata json file: message", ec.message()));
  }

  Metadata metadata(filename, jv);

  spdlog::debug("not prepared");

  metadata.dump();

  metadata.prepare();

  spdlog::debug("prepared");

  metadata.dump();

  return metadata;
}

void Metadata::prepare() {
  for (const auto& f : m_audio_source_filenames) {
    m_audio_archives.push_back(parse_archive(f));
  }

  for (const auto& region : m_regions) {
    region->prepare(m_resolution);
  }
}

std::shared_ptr<Region> Metadata::parseRegion(const std::string& name,
                                              boost::json::object jo) {
  auto cells_excluded_array =
      hisui::util::get_array_from_json_object_with_default(
          jo, "cells_excluded", boost::json::array());
  std::vector<std::uint64_t> cells_excluded;
  for (const auto& v : cells_excluded_array) {
    if (v.is_number()) {
      boost::json::error_code ec;
      auto value = v.to_number<std::uint64_t>(ec);
      if (ec) {
        throw std::runtime_error(
            fmt::format("v.to_number() failed: {}", ec.message()));
      }
      cells_excluded.push_back(value);
    } else {
      throw std::invalid_argument(
          fmt::format("{} contains non-string values", "audio_sources"));
    }
  }

  auto video_sources_array =
      hisui::util::get_array_from_json_object_with_default(
          jo, "video_sources", boost::json::array());
  std::vector<std::string> video_sources;

  for (const auto& v : video_sources_array) {
    if (v.is_string()) {
      video_sources.push_back(std::string(v.as_string()));
    } else {
      throw std::invalid_argument(
          fmt::format("{} contains non-string values", "video_sources"));
    }
  }

  auto reuse_string = hisui::util::get_string_from_json_object_with_default(
      jo, "reuse", "show_oldest");
  Reuse reuse;
  if (reuse_string == "none") {
    reuse = Reuse::None;
  } else if (reuse_string == "show_oldest") {
    reuse = Reuse::ShowOldest;
  } else if (reuse_string == "show_newest") {
    reuse = Reuse::ShowNewest;
  } else {
    throw std::invalid_argument(fmt::format("invalid reuse: {}", reuse_string));
  }

  RegionParameters params{
      .name = name,
      .pos{.x = static_cast<std::uint64_t>(
               hisui::util::get_double_from_json_object_with_default(
                   jo, "x_pos", 0)),
           .y = static_cast<std::uint64_t>(
               hisui::util::get_double_from_json_object_with_default(
                   jo, "y_pos", 0))},
      .z_pos = static_cast<std::int64_t>(
          hisui::util::get_double_from_json_object_with_default(jo, "z_pos",
                                                                0)),
      .resolution{.width = static_cast<std::uint64_t>(
                      hisui::util::get_double_from_json_object_with_default(
                          jo, "width", 0)),
                  .height = static_cast<std::uint64_t>(
                      hisui::util::get_double_from_json_object_with_default(
                          jo, "height", 0))},
      .max_columns = static_cast<std::uint64_t>(
          hisui::util::get_double_from_json_object_with_default(
              jo, "max_columns", 0)),
      .max_rows = static_cast<std::uint64_t>(
          hisui::util::get_double_from_json_object_with_default(jo, "max_rows",
                                                                0)),
      .cells_excluded = cells_excluded,
      .reuse = reuse,
      .video_sources = video_sources,
      .video_sources_excluded = {}  // TODO(haruyama)
  };

  return std::make_shared<Region>(params);
}

}  // namespace hisui::layout

