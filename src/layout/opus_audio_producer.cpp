#include "layout/opus_audio_producer.hpp"

#include <opus_types.h>

#include "audio/basic_sequencer.hpp"
#include "audio/buffer_opus_encoder.hpp"
#include "audio/mixer.hpp"
#include "config.hpp"
#include "layout/audio_sequencer.hpp"
#include "metadata.hpp"
#include "muxer/audio_producer.hpp"

namespace hisui::layout {

OpusAudioProducer::OpusAudioProducer(const hisui::Config& t_config,
                                     const Metadata& t_metadata,
                                     const std::uint64_t timescale)
    : AudioProducer({.show_progress_bar =
                         t_config.show_progress_bar && t_config.audio_only}) {
  switch (t_config.audio_mixer) {
    case hisui::config::AudioMixer::Simple:
      m_mix_sample = hisui::audio::mix_sample_simple;
      break;
    case hisui::config::AudioMixer::Vttoth:
      m_mix_sample = hisui::audio::mix_sample_vttoth;
      break;
  }

  m_sequencer = new hisui::layout::AudioSequencer(t_metadata.getAudioSources());

  m_max_stop_time_offset = t_metadata.getMaxEndTime();

  hisui::audio::BufferOpusEncoder* encoder =
      new hisui::audio::BufferOpusEncoder(
          &m_buffer,
          {.bit_rate = t_config.out_opus_bit_rate, .timescale = timescale});
  m_skip = encoder->getSkip();
  m_encoder = encoder;
}

::opus_int32 OpusAudioProducer::getSkip() const {
  return m_skip;
}

}  // namespace hisui::layout
