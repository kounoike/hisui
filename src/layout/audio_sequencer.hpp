#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "audio/sequencer.hpp"
#include "audio/source.hpp"
#include "layout/audio_source.hpp"
#include "util/interval.hpp"

namespace hisui {

class Archive;

}

namespace hisui::layout {

class AudioSequencer : public hisui::audio::Sequencer {
 public:
  explicit AudioSequencer(const std::vector<std::shared_ptr<AudioSource>>&);

  void getSamples(std::vector<std::pair<std::int16_t, std::int16_t>>*,
                  const std::uint64_t);

 private:
  std::vector<
      std::pair<std::shared_ptr<hisui::audio::Source>, hisui::util::Interval>>
      m_sequence;
};

}  // namespace hisui::layout
