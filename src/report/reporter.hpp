#pragma once

#include <cstdint>
#include <ctime>
#include <map>
#include <string>
#include <tuple>
#include <vector>

#include <boost/json.hpp>
#include <boost/json/object.hpp>

namespace hisui::report {

struct AudioDecoderInfo {
  const std::string codec;
  const int channels;
  const std::int64_t duration;
};

void tag_invoke(const boost::json::value_from_tag&,
                boost::json::value& jv,  // NOLINT
                const AudioDecoderInfo& adi);

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

struct OutputInfo {
  std::string container;
  std::string mux_type;
  std::string video_codec;
  std::string audio_codec;
  double duration;
};

void tag_invoke(const boost::json::value_from_tag&,
                boost::json::value& jv,  // NOLINT
                const OutputInfo& oi);

class Reporter {
 public:
  Reporter& operator=(const Reporter&) = delete;
  Reporter(Reporter&&) = delete;
  Reporter& operator=(Reporter&&) = delete;
  std::string makeSuccessReport();
  void registerOutput(const OutputInfo&);
  void registerAudioDecoder(const std::string&, const AudioDecoderInfo&);
  void registerVideoDecoder(const std::string&, const VideoDecoderInfo&);

  void registerResolutionChange(const std::string&,
                                const ResolutionWithTimestamp&);

  static void open();
  static bool hasInstance();
  static Reporter& getInstance();
  static void close();

 private:
  Reporter();
  ~Reporter() = default;
  void collectVersions();
  std::map<std::string, AudioDecoderInfo> m_audio_decoder_map;
  std::map<std::string, VideoDecoderInfo> m_video_decoder_map;
  std::map<std::string, std::vector<ResolutionWithTimestamp>>
      m_resolution_changes_map;
  OutputInfo m_output_info;

  inline static Reporter* m_reporter = nullptr;
  boost::json::object m_report;
  std::clock_t m_start_clock;
};

}  // namespace hisui::report
