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

#include "util/json.hpp"

namespace hisui::layout {

void Metadata::Dump() const {
  spdlog::debug("format: {}",
                m_format == ContainerFormat::MP4 ? "mp4" : "webm");
  spdlog::debug("bitrate: {}", m_bitrate);
  spdlog::debug("width: {}", m_width);
  spdlog::debug("height: {}", m_height);
  spdlog::debug("trim: {}", m_trim);
}

Metadata::Metadata(const std::string& file_path, const boost::json::value& jv)
    : m_path(file_path) {
  if (m_path.is_relative()) {
    m_path = std::filesystem::absolute(m_path);
  }
  const auto current_path = std::filesystem::current_path();
  std::filesystem::current_path(m_path.parent_path());

  boost::json::object j;
  if (auto p = jv.if_object()) {
    j = *p;
  } else {
    throw std::runtime_error("jv.if_object() failed");
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
    m_width = std::stoull(m[1].str());
    m_height = std::stoull(m[2].str());
  } else {
    throw std::invalid_argument(
        fmt::format("invalid resolution: {}", resolution));
  }
  m_trim = hisui::util::get_bool_from_json_object_with_default(j, "trim", true);
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

  metadata.Dump();

  return metadata;
}

}  // namespace hisui::layout

