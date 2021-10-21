#include "layout/audio_sequencer.hpp"

#include <bits/exception.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <cmath>
#include <filesystem>
#include <string>

#include "audio/webm_source.hpp"
#include "constants.hpp"
#include "metadata.hpp"
#include "util/interval.hpp"

namespace hisui::layout {

AudioSequencer::AudioSequencer(
    const std::vector<std::shared_ptr<AudioSource>>& sources) {
  for (const auto& source : sources) {
    m_sequence.push_back(
        {source->source,
         hisui::util::Interval(
             static_cast<std::uint64_t>(source->source_interval.start_time *
                                        hisui::Constants::PCM_SAMPLE_RATE),
             static_cast<std::uint64_t>(source->source_interval.end_time *
                                        hisui::Constants::PCM_SAMPLE_RATE))});
  }
}

void AudioSequencer::getSamples(
    std::vector<std::pair<std::int16_t, std::int16_t>>* samples,
    const std::uint64_t position) {
  samples->clear();
  for (const auto& s : m_sequence) {
    if (s.second.isIn(position)) {
      samples->push_back(
          s.first->getSample(s.second.getSubstructLower(position)));
    }
  }
}

}  // namespace hisui::layout
