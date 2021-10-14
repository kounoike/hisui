#include "layout/metadata.hpp"

#include <spdlog/spdlog.h>

#include <fstream>
#include <stdexcept>

#include <boost/json/array.hpp>
#include <boost/json/impl/array.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/string.hpp>
#include <boost/json/system_error.hpp>
#include <boost/json/value.hpp>

#include "util/json.hpp"

namespace hisui::layout {

void Metadata::Dump() const {
  spdlog::debug("format: {}",
                m_format == ContainerFormat::MP4 ? "mp4" : "webm");
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

  auto format = hisui::util::get_string_from_json_object(j, "format");
  if (format == "mp4") {
    m_format = ContainerFormat::MP4;
  } else if (format == "webm") {
    m_format = ContainerFormat::WebM;
  } else {
    throw std::invalid_argument(fmt::format("invalid format: {}", format));
  }
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

