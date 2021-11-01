#pragma once

#include <memory>
#include <vector>

#include "config.hpp"
#include "metadata.hpp"
#include "muxer/muxer.hpp"

namespace hisui {

struct Frame;

}

namespace hisui::webm::output {

class Context;

}  // namespace hisui::webm::output

namespace hisui::muxer {

struct AsyncWebMMuxerParameters {
  const std::vector<hisui::Archive>& audio_archives;
  const std::vector<hisui::Archive>& normal_archives;
  const std::vector<hisui::Archive>& preferred_archives;
  const double max_stop_time_offset;
};

struct AsyncWebMMuxerParametersForLayout {
  const std::vector<hisui::Archive>& audio_archives;
  const std::shared_ptr<VideoProducer>& video_producer;
  const double max_stop_time_offset;
};

class AsyncWebMMuxer : public Muxer {
 public:
  AsyncWebMMuxer(const hisui::Config&, const AsyncWebMMuxerParameters&);
  AsyncWebMMuxer(const hisui::Config&,
                 const AsyncWebMMuxerParametersForLayout&);
  ~AsyncWebMMuxer();

  void setUp() override;
  void run() override;
  void cleanUp() override;

 private:
  void muxFinalize() override;
  void appendAudio(hisui::Frame) override;
  void appendVideo(hisui::Frame) override;

  hisui::webm::output::Context* m_context;

  bool has_preferred;
  hisui::Config m_config;
  std::vector<hisui::Archive> m_audio_archives;
  std::vector<hisui::Archive> m_normal_archives;
  std::vector<hisui::Archive> m_preferred_archives;
  double m_max_stop_time_offset;
  std::size_t m_normal_archive_size;
};

}  // namespace hisui::muxer
