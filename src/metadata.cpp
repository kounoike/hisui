#include "metadata.hpp"

#include <bits/exception.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <filesystem>
#include <map>
#include <stdexcept>
#include <tuple>
#include <utility>

#include <boost/json.hpp>

namespace hisui {

Archive::Archive(const std::filesystem::path& t_path,
                 const std::string& m_connection_id,
                 const double t_start_time_offset,
                 const double t_stop_time_offset)
    : m_path(t_path),
      m_connection_id(m_connection_id),
      m_start_time_offset(t_start_time_offset),
      m_stop_time_offset(t_stop_time_offset) {}

std::filesystem::path Archive::getPath() const {
  return m_path;
}

std::string Archive::getConnectionID() const {
  return m_connection_id;
}

double Archive::getStartTimeOffset() const {
  return m_start_time_offset;
}

double Archive::getStopTimeOffset() const {
  return m_stop_time_offset;
}

Metadata::Metadata(const std::string& file_path, const boost::json::value& jv)
    : m_path(file_path) {
  if (m_path.is_relative()) {
    m_path = std::filesystem::absolute(m_path);
  }
  const auto current_path = std::filesystem::current_path();
  std::filesystem::current_path(m_path.parent_path());
  std::vector<std::tuple<std::string, std::string, double, double>> archives;

  auto json_archives = prepare(jv);

  for (const auto& a : json_archives) {
    boost::json::object o;
    if (auto p = a.if_object()) {
      o = *p;
    } else {
      throw std::runtime_error("a.if_object() failed");
    }
    auto a_file_path = get_string_from_json_object(o, "file_path");
    auto a_connection_id = get_string_from_json_object(o, "connection_id");
    double a_start_time_offset =
        get_double_from_json_object(o, "start_time_offset");
    double a_stop_time_offset =
        get_double_from_json_object(o, "stop_time_offset");
    spdlog::debug("{} {} {} {}", a_file_path, a_connection_id,
                  a_start_time_offset, a_stop_time_offset);
    archives.emplace_back(a_file_path, a_connection_id, a_start_time_offset,
                          a_stop_time_offset);
  }
  std::sort(archives.begin(), archives.end(),
            [](const std::tuple<std::string, std::string, double, double>& a,
               const std::tuple<std::string, std::string, double, double>& b) {
              if (get<2>(a) != get<2>(b)) {
                // 開始時間が先のものを優先する
                return get<2>(a) < get<2>(b);
              }
              if (get<3>(a) != get<3>(b)) {
                // 終了時間が後のものを優先する
                return get<3>(a) > get<3>(b);
              }
              if (get<1>(a) != get<1>(b)) {
                return get<1>(a) < get<1>(b);
              }
              if (get<0>(a) != get<0>(b)) {
                return get<0>(a) < get<0>(b);
              }
              return false;
            });

  for (const auto& a : archives) {
    std::filesystem::path path(get<0>(a));
    if (path.is_relative()) {
      path = std::filesystem::absolute(path);
    }
    if (!std::filesystem::exists(path)) {
      spdlog::debug("file is not found(1). try relative path: {}",
                    path.string());
      path = std::filesystem::absolute(path.filename());
      if (!std::filesystem::exists(path)) {
        spdlog::debug("file is not found(2): {}", path.string());
        throw std::runtime_error(
            fmt::format("file is not found: {}", get<0>(a)));
      }
    }
    Archive archive(path, get<1>(a), get<2>(a), get<3>(a));
    m_archives.push_back(archive);
    if (get<2>(a) < m_min_start_time_offset) {
      m_min_start_time_offset = get<2>(a);
    }
    if (get<3>(a) > m_max_stop_time_offset) {
      m_max_stop_time_offset = get<3>(a);
    }
  }
  std::filesystem::current_path(current_path);
}

std::vector<Archive> Metadata::getArchives() const {
  return m_archives;
}

double Metadata::getMinStartTimeOffset() const {
  return m_min_start_time_offset;
}

double Metadata::getMaxStopTimeOffset() const {
  return m_max_stop_time_offset;
}

boost::json::array Metadata::prepare(const boost::json::value& jv) {
  boost::json::object j;
  if (auto p = jv.if_object()) {
    j = *p;
  } else {
    throw std::runtime_error("jv.if_object() failed");
  }

  try {
    m_recording_id = get_string_from_json_object(j, "recording_id");
  } catch (const std::exception& e) {
    spdlog::error("invalid format: cannot get recording_id: what={}", e.what());
    throw e;
  }

  if (j["archives"] == nullptr) {
    throw std::invalid_argument("not metadata json file: {}");
  }

  boost::json::array ja;
  if (auto p = j["archives"].if_array()) {
    ja = *p;
  } else {
    throw std::runtime_error("if_array() failed");
  }

  if (std::size(ja) == 0) {
    throw std::invalid_argument("metadata json file does not include archives");
  }
  return ja;
}

Metadata parse_metadata(const std::string& filename,
                        const boost::json::value& jv) {
  Metadata metadata(filename, jv);

  spdlog::debug("metadata min_start_time_offset={}",
                metadata.getMinStartTimeOffset());
  spdlog::debug("metadata max_start_time_offset={}",
                metadata.getMaxStopTimeOffset());
  for (const auto& archive : metadata.getArchives()) {
    spdlog::debug("  file_path='{} start_time_offset={} stop_time_offset={}",
                  archive.getPath().string(), archive.getStartTimeOffset(),
                  archive.getStopTimeOffset());
    metadata.getMaxStopTimeOffset();
  }

  return metadata;
}

boost::json::string Metadata::getRecordingID() const {
  return m_recording_id;
}

boost::json::string get_string_from_json_object(boost::json::object o,
                                                const std::string& key) {
  if (auto p = o[key].if_string()) {
    return *p;
  }
  throw std::runtime_error(fmt::format("o[{}].if_string() failed", key));
}

double get_double_from_json_object(boost::json::object o,
                                   const std::string& key) {
  if (o[key].is_number()) {
    boost::json::error_code ec;
    auto value = o[key].to_number<double>(ec);
    if (ec) {
      throw std::runtime_error(
          fmt::format("o[{}].to_number() failed: {}", key, ec.message()));
    }
    return value;
  }
  throw std::runtime_error("start_time_offset is not number");
}

}  // namespace hisui
