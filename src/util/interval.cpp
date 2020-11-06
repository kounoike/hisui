#include "util/interval.hpp"

namespace hisui::util {

Interval::Interval(const std::uint64_t t_lower, const std::uint64_t t_upper)
    : m_lower(t_lower), m_upper(t_upper) {}

bool Interval::isIn(const std::uint64_t t) const {
  return m_lower <= t && t < m_upper;
}
std::uint64_t Interval::getSubstructLower(const std::uint64_t t) const {
  return t - m_lower;
}

}  // namespace hisui::util
