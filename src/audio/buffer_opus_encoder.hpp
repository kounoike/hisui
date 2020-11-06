#pragma once

#include <opus.h>
#include <opus_types.h>

#include <cstdint>
#include <queue>
#include <vector>

#include "audio/encoder.hpp"
#include "constants.hpp"
#include "webm/output/context.hpp"

namespace hisui::audio {

class BufferOpusEncoder : public Encoder {
 public:
  explicit BufferOpusEncoder(std::queue<hisui::webm::output::FrameTuple>*);
  ~BufferOpusEncoder();
  void addSample(const std::int16_t, const std::int16_t);
  void flush();

  ::opus_int32 getSkip() const;

 private:
  std::queue<hisui::webm::output::FrameTuple>* m_buffer;
  ::OpusEncoder* m_encoder;
  std::vector<opus_int16> m_pcm_buffer;
  std::uint8_t m_opus_buffer[hisui::Constants::OPUS_MAX_PACKET_SIZE];
  std::uint64_t m_timestamp = 0;
  const std::uint64_t m_timestamp_step =
      static_cast<std::uint64_t>(hisui::Constants::OPUS_ENCODE_FRAME_SIZE) *
      hisui::Constants::NANO_SECOND / hisui::Constants::PCM_SAMPLE_RATE;
  ::opus_int32 m_skip;

  void encodeAndWrite();
};

}  // namespace hisui::audio
