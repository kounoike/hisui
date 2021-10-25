#pragma once

#include <memory>
#include <vector>

#include <layout/region.hpp>

namespace hisui::layout {

struct ComposerParameters {
  const std::vector<std::shared_ptr<Region>>& regions;
  const Resolution& resolution;
};

class Composer {
 public:
  explicit Composer(const ComposerParameters&);
  ~Composer();
  void compose(std::vector<unsigned char>*, const std::uint64_t);

 private:
  std::vector<std::shared_ptr<Region>> m_regions;
  Resolution m_resolution;

  std::array<unsigned char*, 3> m_planes;
  std::array<std::size_t, 3> m_plane_sizes;
  std::array<unsigned char, 3> m_plane_default_values;
};
}  // namespace hisui::layout
