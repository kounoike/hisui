#pragma once

#include <vector>

#include "archive.hpp"

namespace hisui {

class AudioMetadata {
 public:
  virtual ~AudioMetadata() {}
  virtual double getMaxStopTimeOffset() const = 0;
  virtual std::vector<Archive> getAudioArchives() const = 0;
};

}  // namespace hisui
