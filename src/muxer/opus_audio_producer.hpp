#pragma once

#include <opus_types.h>

#include <cstdint>

#include "constants.hpp"
#include "muxer/audio_producer.hpp"

namespace hisui {

class Config;
class AudioMetadata;

}  // namespace hisui

namespace hisui::muxer {

class OpusAudioProducer : public AudioProducer {
 public:
  OpusAudioProducer(
      const hisui::Config&,
      const hisui::AudioMetadata&,
      const std::uint64_t timescale = hisui::Constants::NANO_SECOND);
  ::opus_int32 getSkip() const;

 private:
  ::opus_int32 m_skip;
};

}  // namespace hisui::muxer
