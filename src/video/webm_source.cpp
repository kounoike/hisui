#include "video/webm_source.hpp"

#include <bits/exception.h>

#include <cstdio>
#include <stdexcept>

#include "fmt/core.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/spdlog.h"

#include "constants.hpp"
#include "video/decoder.hpp"
#include "video/vpx_decoder.hpp"
#include "video/yuv.hpp"
#include "webm/input/video_context.hpp"

#ifdef USE_OPENH264
#include "video/h264_decoder.hpp"
#endif

namespace hisui::video {

WebMSource::WebMSource(const std::string& file_path) {
  m_file = std::fopen(file_path.c_str(), "rb");
  if (m_file == nullptr) {
    throw std::runtime_error("Unable to open: " + file_path);
  }

  m_webm = new hisui::webm::input::VideoContext();
  if (!m_webm->init(m_file)) {
    spdlog::info(
        "VideoContext initialization failed. no video track or unsupported "
        "codec: file_path={}",
        file_path);

    std::fclose(m_file);
    delete m_webm;
    m_webm = nullptr;
    m_width = 320;
    m_height = 240;
    m_black_yuv_image = create_black_yuv_image(m_width, m_height);
    return;
  }

  m_width = m_webm->getWidth();
  m_height = m_webm->getHeight();

  spdlog::trace("WebMSource: file_path={}, width={}, height={}", file_path,
                m_width, m_height);

  m_duration = static_cast<std::uint64_t>(m_webm->getDuration());
  m_black_yuv_image = create_black_yuv_image(m_width, m_height);

  switch (m_webm->getFourcc()) {
    case hisui::Constants::VP8_FOURCC: /* fall through */
    case hisui::Constants::VP9_FOURCC:
      m_decoder = new VPXDecoder(m_webm);
      break;
#ifdef USE_OPENH264
    case hisui::Constants::H264_FOURCC:
      m_decoder = new H264Decoder(m_webm);
      break;
#endif
    default:
      auto fourcc = m_webm->getFourcc();
      std::fclose(m_file);
      delete m_webm;
      m_webm = nullptr;
      throw std::runtime_error(fmt::format("unknown fourcc: {}", fourcc));
  }
}

WebMSource::~WebMSource() {
  if (m_webm) {
    delete m_webm;
  }
  std::fclose(m_file);
  if (m_decoder) {
    delete m_decoder;
  }
  if (m_black_yuv_image) {
    delete m_black_yuv_image;
  }
}

const YUVImage* WebMSource::getYUV(const std::uint64_t timestamp) {
  if (!m_decoder) {
    return m_black_yuv_image;
  }
  return m_decoder->getImage(timestamp);
}

std::uint32_t WebMSource::getWidth() const {
  return m_width;
}
std::uint32_t WebMSource::getHeight() const {
  return m_height;
}

}  // namespace hisui::video
