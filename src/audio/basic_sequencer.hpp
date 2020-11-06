#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "audio/sequencer.hpp"
#include "audio/source.hpp"

namespace hisui {

class Archive;

}

namespace hisui::util {

class Interval;

}

namespace hisui::audio {

class BasicSequencer : public Sequencer {
 public:
  explicit BasicSequencer(const std::vector<hisui::Archive>&);

  void getSamples(std::vector<std::pair<std::int16_t, std::int16_t>>*,
                  const std::uint64_t);

 private:
  std::vector<
      std::pair<std::unique_ptr<hisui::audio::Source>, hisui::util::Interval>>
      m_sequence;
};

}  // namespace hisui::audio
