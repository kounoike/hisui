#include "muxer/video_producer.hpp"

#include <cmath>
#include <cstdint>
#include <mutex>
#include <optional>
#include <vector>

#include "boost/rational.hpp"
#include "progresscpp/ProgressBar.hpp"

#include "constants.hpp"
#include "util/interval.hpp"
#include "video/basic_sequencer.hpp"
#include "video/buffer_vpx_encoder.hpp"
#include "video/composer.hpp"
#include "video/encoder.hpp"
#include "video/grid_composer.hpp"
#include "video/parallel_grid_composer.hpp"
#include "video/sequencer.hpp"
#include "video/source.hpp"
#include "video/vpx.hpp"

namespace hisui::video {

class YUVImage;

}

namespace hisui::muxer {

VideoProducer::VideoProducer(const hisui::Config& t_config,
                             const hisui::Metadata& t_metadata,
                             hisui::webm::output::Context* t_context)
    : m_config(t_config), m_metadata(t_metadata), m_context(t_context) {
  m_show_progress_bar = m_config.show_progress_bar;

  m_sequencer = new hisui::video::BasicSequencer(m_metadata.getArchives());

  auto scaling_width = m_config.scaling_width != 0 ? m_config.scaling_width
                                                   : m_sequencer->getMaxWidth();
  auto scaling_height = m_config.scaling_height != 0
                            ? m_config.scaling_height
                            : m_sequencer->getMaxHeight();

  switch (m_config.video_composer) {
    case hisui::config::VideoComposer::Grid:
      m_composer = new hisui::video::GridComposer(
          scaling_width, scaling_height, m_sequencer->getSize(),
          m_config.max_columns, m_config.video_scaler,
          m_config.libyuv_filter_mode);
      break;
    case hisui::config::VideoComposer::ParallelGrid:
      m_composer = new hisui::video::ParallelGridComposer(
          scaling_width, scaling_height, m_sequencer->getSize(),
          m_config.max_columns, m_config.video_scaler,
          m_config.libyuv_filter_mode);
      break;
  }

  hisui::video::VPXEncoderConfig vpx_config(m_composer->getWidth(),
                                            m_composer->getHeight(), m_config);

  m_encoder = new hisui::video::BufferVPXEncoder(&m_buffer, vpx_config);

  m_context->setVideoTrack(m_composer->getWidth(), m_composer->getHeight(),
                           m_encoder->getFourcc());
}

VideoProducer::~VideoProducer() {
  delete m_encoder;
  delete m_sequencer;
  delete m_composer;
}

void VideoProducer::produce() {
  std::vector<const video::YUVImage*> yuvs;
  std::vector<unsigned char> raw_image;
  yuvs.resize(m_sequencer->getSize());
  raw_image.resize(m_composer->getWidth() * m_composer->getHeight() * 3 >> 1);

  std::uint64_t max_time = static_cast<std::uint64_t>(std::ceil(
      m_metadata.getMaxStopTimeOffset() * hisui::Constants::NANO_SECOND));

  progresscpp::ProgressBar progress_bar(max_time, 60);

  for (std::uint64_t t = 0, step = hisui::Constants::NANO_SECOND *
                                   m_config.out_video_frame_rate.denominator() /
                                   m_config.out_video_frame_rate.numerator();
       t < max_time; t += step) {
    m_sequencer->getYUVs(&yuvs, t);
    m_composer->compose(&raw_image, yuvs);
    {
      std::lock_guard<std::mutex> lock(m_mutex_buffer);
      m_encoder->outputImage(raw_image);
    }

    if (m_show_progress_bar) {
      progress_bar.setTicks(t);
      progress_bar.display();
    }
  }

  {
    std::lock_guard<std::mutex> lock(m_mutex_buffer);
    m_encoder->flush();
    m_is_finished = true;
  }

  if (m_show_progress_bar) {
    progress_bar.setTicks(max_time);
    progress_bar.done();
  }
}

void VideoProducer::bufferPop() {
  std::lock_guard<std::mutex> lock(m_mutex_buffer);
  m_buffer.pop();
}

std::optional<hisui::webm::output::FrameTuple> VideoProducer::bufferFront() {
  std::lock_guard<std::mutex> lock(m_mutex_buffer);
  if (m_buffer.empty()) {
    return {};
  }
  return m_buffer.front();
}

bool VideoProducer::isFinished() {
  std::lock_guard<std::mutex> lock(m_mutex_buffer);
  return m_is_finished && m_buffer.empty();
}

}  // namespace hisui::muxer
