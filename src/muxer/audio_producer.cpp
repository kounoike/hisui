#include "muxer/audio_producer.hpp"

#include <array>
#include <cmath>
#include <iterator>
#include <mutex>
#include <utility>
#include <vector>

#include "audio/basic_sequencer.hpp"
#include "audio/buffer_opus_encoder.hpp"
#include "audio/encoder.hpp"
#include "audio/mixer.hpp"
#include "audio/opus.hpp"
#include "audio/sequencer.hpp"
#include "constants.hpp"

namespace hisui::muxer {

AudioProducer::AudioProducer(const hisui::Config& t_config,
                             const hisui::Metadata& t_metadata,
                             hisui::webm::output::Context* t_context)
    : m_config(t_config), m_metadata(t_metadata), m_context(t_context) {
  switch (m_config.audio_mixer) {
    case hisui::config::AudioMixer::Simple:
      m_mix_sample = hisui::audio::mix_sample_simple;
      break;
    case hisui::config::AudioMixer::Vttoth:
      m_mix_sample = hisui::audio::mix_sample_vttoth;
      break;
  }

  m_sequencer = new hisui::audio::BasicSequencer(m_metadata.getArchives());

  m_encoder = new hisui::audio::BufferOpusEncoder(&m_buffer);

  auto skip =
      dynamic_cast<hisui::audio::BufferOpusEncoder*>(m_encoder)->getSkip();
  auto private_data = hisui::audio::create_opus_private_data(
      skip, hisui::Constants::PCM_SAMPLE_RATE);

  m_context->setAudioTrack(static_cast<std::uint64_t>(skip) *
                               hisui::Constants::NANO_SECOND /
                               hisui::Constants::PCM_SAMPLE_RATE,
                           private_data.data(), std::size(private_data));
}

AudioProducer::~AudioProducer() {
  delete m_sequencer;
  delete m_encoder;
}

void AudioProducer::produce() {
  std::vector<std::pair<std::int16_t, std::int16_t>> samples;

  for (std::uint64_t p = 0, m = static_cast<std::uint64_t>(
                                std::ceil(m_metadata.getMaxStopTimeOffset() *
                                          hisui::Constants::PCM_SAMPLE_RATE));
       p < m; ++p) {
    std::int16_t left = 0;
    std::int16_t right = 0;
    m_sequencer->getSamples(&samples, p);
    for (const auto& s : samples) {
      auto [l, r] = s;
      if (l != 0) {
        left = m_mix_sample(left, l);
      }
      if (r != 0) {
        right = m_mix_sample(right, r);
      }
    }
    {
      std::lock_guard<std::mutex> lock(m_mutex_buffer);
      m_encoder->addSample(left, right);
    }
  }

  {
    std::lock_guard<std::mutex> lock(m_mutex_buffer);
    m_encoder->flush();
    m_is_finished = true;
  }
}

void AudioProducer::bufferPop() {
  std::lock_guard<std::mutex> lock(m_mutex_buffer);
  m_buffer.pop();
}

std::optional<hisui::webm::output::FrameTuple> AudioProducer::bufferFront() {
  std::lock_guard<std::mutex> lock(m_mutex_buffer);
  if (m_buffer.empty()) {
    return {};
  }
  return m_buffer.front();
}

bool AudioProducer::isFinished() {
  std::lock_guard<std::mutex> lock(m_mutex_buffer);
  return m_is_finished && m_buffer.empty();
}

}  // namespace hisui::muxer
