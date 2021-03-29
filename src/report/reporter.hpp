#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include <boost/json.hpp>
#include <boost/json/object.hpp>

namespace hisui::report {

struct VideoDecoderInfo {
  const std::string codec;
  const std::int64_t duration;
};

void tag_invoke(const boost::json::value_from_tag&,
                boost::json::value& jv,  // NOLINT
                const VideoDecoderInfo& vdi);

struct ResolutionWithTimestamp {
  const std::uint64_t timestamp;
  const std::uint32_t width;
  const std::uint32_t height;
};

void tag_invoke(const boost::json::value_from_tag&,
                boost::json::value& jv,  // NOLINT
                const ResolutionWithTimestamp& rwt);

class Reporter {
 public:
  Reporter& operator=(const Reporter&) = delete;
  Reporter(Reporter&&) = delete;
  Reporter& operator=(Reporter&&) = delete;
  std::string makeSuccessReport();
  void registerVideoDecoder(const std::string&, const VideoDecoderInfo&);

  void registerResolutionChange(const std::string&,
                                const ResolutionWithTimestamp&);

  static void open();
  static bool hasInstance();
  static Reporter& getInstance();
  static void close();

 private:
  Reporter() = default;
  ~Reporter() = default;
  void collectVersions();
  std::map<std::string, VideoDecoderInfo> m_video_decoder_map;
  std::map<std::string, std::vector<ResolutionWithTimestamp>>
      m_resolution_changes_map;

  boost::json::object report;
  inline static Reporter* m_reporter = nullptr;
};

}  // namespace hisui::report
