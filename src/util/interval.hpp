#pragma once

#include <cstdint>

namespace hisui::util {

class Interval {
 public:
  Interval(const std::uint64_t, const std::uint64_t);

  bool isIn(const std::uint64_t) const;
  std::uint64_t getSubstructLower(const std::uint64_t) const;

 private:
  std::uint64_t m_lower;
  std::uint64_t m_upper;
};

}  // namespace hisui::util
