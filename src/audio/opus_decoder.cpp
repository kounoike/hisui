#include "audio/opus_decoder.hpp"

#include <fmt/core.h>
#include <opus_defines.h>
#include <opus_types.h>

#include <stdexcept>

#include "audio/opus.hpp"
#include "constants.hpp"

namespace hisui::audio {

OpusDecoder::OpusDecoder(const int t_channles) : m_channels(t_channles) {
  if (m_channels != 1 && m_channels != 2) {
    throw std::invalid_argument(
        fmt::format("invalid number of channels: {}", m_channels));
  }

  m_decoder = create_opus_decoder(hisui::Constants::PCM_SAMPLE_RATE,
                                  static_cast<int>(m_channels));

  m_opus_buffer = new std::int16_t[static_cast<std::size_t>(
      hisui::Constants::OPUS_DECODE_MAX_FRAME_SIZE *
      static_cast<std::uint32_t>(m_channels))];
}

OpusDecoder::~OpusDecoder() {
  if (m_opus_buffer) {
    delete[] m_opus_buffer;
  }
  if (m_decoder) {
    ::opus_decoder_destroy(m_decoder);
  }
}

std::pair<const std::int16_t*, const std::size_t> OpusDecoder::decode(
    const unsigned char* src_buffer,
    const std::size_t src_buffer_length) {
  const int number_of_samples = ::opus_decode(
      m_decoder, src_buffer, static_cast<opus_int32>(src_buffer_length),
      m_opus_buffer, hisui::Constants::OPUS_DECODE_MAX_FRAME_SIZE, 0);

  if (number_of_samples < 0) {
    throw std::runtime_error(fmt::format("opus_decode() failed: error='{}'",
                                         ::opus_strerror(number_of_samples)));
  }
  return {m_opus_buffer, number_of_samples * m_channels};
}

}  // namespace hisui::audio
