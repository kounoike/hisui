#pragma once

#include <opus_types.h>

#include <cstdint>

#include "constants.hpp"
#include "layout/metadata.hpp"
#include "muxer/audio_producer.hpp"

namespace hisui {

class Config;

}  // namespace hisui

namespace hisui::layout {

class OpusAudioProducer : public hisui::muxer::AudioProducer {
 public:
  OpusAudioProducer(
      const hisui::Config&,
      const Metadata&,
      const std::uint64_t timescale = hisui::Constants::NANO_SECOND);
  ::opus_int32 getSkip() const;

 private:
  ::opus_int32 m_skip;
};

}  // namespace hisui::layout
