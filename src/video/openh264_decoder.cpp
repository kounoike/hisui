#include "video/openh264_decoder.hpp"

#include <codec/api/svc/codec_api.h>
#include <codec/api/svc/codec_app_def.h>
#include <codec/api/svc/codec_def.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <limits>
#include <stdexcept>

#include "video/openh264.hpp"
#include "video/openh264_handler.hpp"
#include "video/yuv.hpp"
#include "webm/input/video_context.hpp"

namespace hisui::video {

OpenH264Decoder::OpenH264Decoder(hisui::webm::input::VideoContext* t_webm)
    : Decoder(t_webm) {
  const auto create_decoder_ret =
      OpenH264Handler::getInstance().createDecoder(&m_decoder);
  if (create_decoder_ret != 0 || m_decoder == nullptr) {
    throw std::runtime_error(
        fmt::format("m_h264_handler->createDecoder() failed: error_code={}",
                    create_decoder_ret));
  }
  ::SDecodingParam param;
  param.pFileNameRestructed = nullptr;
  param.uiCpuLoad = 100;  // openh264 のソースをみると利用していないようだ
  param.uiTargetDqLayer = 1;
  param.eEcActiveIdc = ::ERROR_CON_DISABLE;
  param.bParseOnly = false;
  param.sVideoProperty.eVideoBsType = ::VIDEO_BITSTREAM_AVC;
  const auto decoder_initialize_ret = m_decoder->Initialize(&param);
  if (decoder_initialize_ret != 0) {
    throw std::runtime_error(
        fmt::format("m_decoder->Initialize() failed: error_code={}",
                    decoder_initialize_ret));
  }

  m_current_yuv_image = create_black_yuv_image(m_width, m_height);
  m_next_yuv_image = create_black_yuv_image(m_width, m_height);

  m_tmp_yuv[0] = nullptr;
  m_tmp_yuv[1] = nullptr;
  m_tmp_yuv[2] = nullptr;
}

OpenH264Decoder::~OpenH264Decoder() {
  if (m_next_yuv_image && m_next_yuv_image != m_current_yuv_image) {
    delete m_next_yuv_image;
  }
  if (m_current_yuv_image) {
    delete m_current_yuv_image;
  }
  if (m_decoder) {
    m_decoder->Uninitialize();
    OpenH264Handler::getInstance().destroyDecoder(m_decoder);
  }
}

const YUVImage* OpenH264Decoder::getImage(const std::uint64_t timestamp) {
  // 非対応 WebM or 時間超過
  if (!m_webm || m_is_time_over) {
    return m_black_yuv_image;
  }
  // 時間超過した
  if (m_duration <= timestamp) {
    m_is_time_over = true;
    return m_black_yuv_image;
  }
  updateImage(timestamp);
  return m_current_yuv_image;
}

void OpenH264Decoder::updateImage(const std::uint64_t timestamp) {
  // 次のブロックに逹っしていない
  if (timestamp < m_next_timestamp) {
    return;
  }
  // 次以降のブロックに逹っした
  updateImageByTimestamp(timestamp);
}

void OpenH264Decoder::updateImageByTimestamp(const std::uint64_t timestamp) {
  if (m_finished_webm) {
    return;
  }

  do {
    if (m_current_yuv_image && m_current_yuv_image != m_next_yuv_image) {
      delete m_current_yuv_image;
    }
    m_current_yuv_image = m_next_yuv_image;
    m_current_timestamp = m_next_timestamp;
    if (m_webm->readFrame()) {
      ::SBufferInfo buffer_info;
      const auto ret = m_decoder->DecodeFrameNoDelay(
          m_webm->getBuffer(), static_cast<int>(m_webm->getBufferSize()),
          m_tmp_yuv, &buffer_info);
      if (ret != 0) {
        spdlog::error(
            "OpenH264Decoder DecodeFrameNoDelay failed: error_code={}", ret);
        throw std::runtime_error(fmt::format(
            "m_decoder->DecodeFrameNoDelay() failed: error_code={}", ret));
      }
      m_next_timestamp = static_cast<std::uint64_t>(m_webm->getTimestamp());
      if (buffer_info.iBufferStatus == 1) {
        m_next_yuv_image = new YUVImage(m_width, m_height);
        update_yuv_image_by_openh264_buffer_info(m_next_yuv_image, buffer_info);
      }
    } else {
      // m_duration までは m_current_image を出すので webm を読み終えても m_current_image を維持する
      m_finished_webm = true;
      m_next_timestamp = std::numeric_limits<std::uint64_t>::max();
      return;
    }
  } while (timestamp >= m_next_timestamp);
}

}  // namespace hisui::video
