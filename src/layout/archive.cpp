#include "layout/archive.hpp"

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include <boost/json/array.hpp>
#include <boost/json/impl/array.hpp>
#include <boost/json/object.hpp>
#include <boost/json/parse.hpp>
#include <boost/json/string.hpp>
#include <boost/json/system_error.hpp>
#include <boost/json/value.hpp>

#include "util/json.hpp"

namespace hisui::layout {

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

std::shared_ptr<Archive> parse_archive(const std::string& filename) {
  auto json_path_result = find_file(filename);
  if (!json_path_result.found) {
    throw std::invalid_argument(json_path_result.message);
  }
  std::ifstream i(json_path_result.path);
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

  return std::make_shared<Archive>(json_path_result.path, jv);
}

Archive::Archive(const std::filesystem::path& t_path,
                 const boost::json::value& jv)
    : m_path(t_path) {
  const auto current_path = std::filesystem::current_path();
  std::filesystem::current_path(m_path.parent_path());

  boost::json::object j;
  if (jv.is_object()) {
    j = jv.as_object();
  } else {
    throw std::runtime_error("jv is not object");
  }

  m_connection_id =
      hisui::util::get_string_from_json_object(j, "connection_id");
  m_start_time = hisui::util::get_double_from_json_object(j, "start_time");
  m_stop_time = hisui::util::get_double_from_json_object(j, "stop_time");

  auto filename = hisui::util::get_string_from_json_object(j, "filename");
  auto filename_result = find_file(std::string(filename));
  if (filename_result.found) {
    m_file_path = filename_result.path;
  } else {
    auto file_path = hisui::util::get_string_from_json_object(j, "file_path");
    auto file_path_result = find_file(std::string(file_path));
    if (file_path_result.found) {
      m_file_path = file_path_result.path;
    } else {
      throw std::invalid_argument(fmt::format(
          "filename() and file_path() do not exsit", filename, file_path));
    }
  }

  std::filesystem::current_path(current_path);
}

void Archive::dump() const {
  spdlog::debug("path: {}", m_path.string());
  spdlog::debug("connection_id: {}", m_connection_id);
  spdlog::debug("start_time: {}", m_start_time);
  spdlog::debug("stop_time: {}", m_stop_time);
  spdlog::debug("file_path: {}", m_file_path.string());
}

void Archive::prepare() {}

const SourceParameters Archive::getSourceParameters(
    const std::size_t index) const {
  return SourceParameters{
      .file_path = m_file_path,
      .index = index,
      .connection_id = m_connection_id,
      .start_time = m_start_time,
      .end_time = m_stop_time,
  };
}

}  // namespace hisui::layout
