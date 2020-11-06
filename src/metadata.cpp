#include "metadata.hpp"

#include <bits/exception.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <filesystem>
#include <map>
#include <stdexcept>
#include <tuple>

#include <nlohmann/json.hpp>

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

Metadata::Metadata(const std::string& file_path,
                   const nlohmann::json& t_raw_json)
    : m_path(file_path), m_raw_json(t_raw_json) {
  if (m_path.is_relative()) {
    m_path = std::filesystem::absolute(m_path);
  }
  const auto current_path = std::filesystem::current_path();
  std::filesystem::current_path(m_path.parent_path());
  std::vector<std::tuple<std::string, std::string, double, double>> archives;
  for (const auto& a : m_raw_json["archives"]) {
    archives.emplace_back(a["file_path"], a["connection_id"],
                          a["start_time_offset"], a["stop_time_offset"]);
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

nlohmann::json Metadata::getRawJSON() const {
  return m_raw_json;
}

double Metadata::getMinStartTimeOffset() const {
  return m_min_start_time_offset;
}

double Metadata::getMaxStopTimeOffset() const {
  return m_max_stop_time_offset;
}

Metadata parse_metadata(const std::string& filename, const nlohmann::json& j) {
  Metadata metadata(filename, j);

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

}  // namespace hisui
