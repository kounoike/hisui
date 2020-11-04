#include "audio/buffer_opus_encoder.hpp"

#include <bits/exception.h>
#include <fmt/core.h>
#include <opus_defines.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>

#include "audio/opus.hpp"

namespace hisui::audio {

BufferOpusEncoder::BufferOpusEncoder(
    std::queue<hisui::webm::output::FrameTuple>* t_buffer)
    : m_buffer(t_buffer) {
  m_encoder = create_opus_encoder(hisui::Constants::PCM_SAMPLE_RATE, 2,
                                  hisui::Constants::OPUS_BITRATE);

  const int ret = ::opus_encoder_ctl(m_encoder, OPUS_GET_LOOKAHEAD(&m_skip));
  if (ret < 0) {
    throw std::runtime_error(
        fmt::format("opus_encoder_ctl(GET_LOOKAHEAD) failed: error='{}'",
                    ::opus_strerror(ret)));
  }

  spdlog::debug("BufferOpusEncoder: skip={}", m_skip);
}

BufferOpusEncoder::~BufferOpusEncoder() {
  ::opus_encoder_destroy(m_encoder);
}

void BufferOpusEncoder::addSample(const std::int16_t left,
                                  const std::int16_t right) {
  m_pcm_buffer.push_back(left);
  m_pcm_buffer.push_back(right);

  if (std::size(m_pcm_buffer) >= hisui::Constants::OPUS_ENCODE_FRAME_SIZE * 2) {
    encodeAndWrite();
    m_timestamp += m_timestamp_step;
  }
}

void BufferOpusEncoder::flush() {
  // 1 sample しかない場合に実施すると ::OPUS_BUFFER_TOO_SMALL が返るので, その場合は切り捨てる
  auto size = std::size(m_pcm_buffer);
  if (size > 2) {
    encodeAndWrite();
  } else if (size > 0) {
    spdlog::debug("pcm buffer has only 1 sample. cannot encode");
  }
}

::opus_int32 BufferOpusEncoder::getSkip() const {
  return m_skip;
}

void BufferOpusEncoder::encodeAndWrite() {
  int number_of_bytes =
      ::opus_encode(m_encoder, m_pcm_buffer.data(),
                    static_cast<int>(std::size(m_pcm_buffer) / 2),
                    m_opus_buffer, hisui::Constants::OPUS_MAX_PACKET_SIZE);
  if (number_of_bytes < 0) {
    throw std::runtime_error(fmt::format("opus_encode() failed: error='{}'",
                                         opus_strerror(number_of_bytes)));
  }

  std::uint8_t* data =
      new std::uint8_t[static_cast<std::size_t>(number_of_bytes)];
  std::copy(m_opus_buffer, m_opus_buffer + number_of_bytes, data);
  m_buffer->emplace(m_timestamp, data, number_of_bytes, true);

  m_pcm_buffer.clear();
}

}  // namespace hisui::audio
