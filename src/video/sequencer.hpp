#pragma once

#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace hisui::util {

class Interval;

}

namespace hisui::video {

class Source;
class YUVImage;

using SourceAndInterval =
    std::pair<std::unique_ptr<Source>, hisui::util::Interval>;

class Sequencer {
 public:
  virtual ~Sequencer() = default;

  virtual void getYUVs(std::vector<const YUVImage*>*, const std::uint64_t) = 0;

  std::uint32_t getMaxWidth() const;
  std::uint32_t getMaxHeight() const;
  std::size_t getSize() const;

 protected:
  std::vector<
      std::pair<std::string, std::shared_ptr<std::vector<SourceAndInterval>>>>
      m_sequence;
  std::uint32_t m_max_width;
  std::uint32_t m_max_height;
  std::size_t m_size;
};

}  // namespace hisui::video
