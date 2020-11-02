#include "video/basic_sequencer.hpp"

#include <bits/exception.h>

#include <cmath>
#include <cstddef>
#include <filesystem>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>

#include "spdlog/fmt/fmt.h"
#include "spdlog/spdlog.h"

#include "constants.hpp"
#include "metadata.hpp"
#include "util/interval.hpp"
#include "video/image_source.hpp"
#include "video/source.hpp"
#include "video/webm_source.hpp"
#include "video/yuv.hpp"

namespace hisui::video {

BasicSequencer::BasicSequencer(const std::vector<hisui::Archive>& archives) {
  m_max_width = 0;
  m_max_height = 0;

  const std::set<std::string> image_extensions({".png", ".jpg", ".jpeg"});

  for (const auto& archive : archives) {
    Source* source;
    const auto& path = archive.getPath();
    const auto extension = path.extension();
    if (extension == ".webm") {
      source = new WebMSource(path.string());
    } else if (image_extensions.contains(extension)) {
      source = new ImageSource(path.string());
    } else {
      spdlog::info("unsupported video source: {}", path.string());
      continue;
    }
    const auto width = source->getWidth();
    const auto height = source->getHeight();
    if (width > m_max_width) {
      m_max_width = width;
    }
    if (height > m_max_height) {
      m_max_height = height;
    }
    const auto connection_id = archive.getConnectionID();
    if (!m_sequence.contains(connection_id)) {
      auto v = std::make_unique<std::vector<SourceAndInterval>>();
      m_sequence[connection_id] = std::move(v);
    }
    m_sequence[connection_id]->push_back(
        {std::unique_ptr<Source>(source),
         hisui::util::Interval(
             static_cast<std::uint64_t>(std::floor(
                 archive.getStartTimeOffset() * hisui::Constants::NANO_SECOND)),
             static_cast<std::uint64_t>(
                 std::ceil(archive.getStopTimeOffset() *
                           hisui::Constants::NANO_SECOND)))});
  }

  m_size = std::size(m_sequence);

  // codec には奇数をあたえるとおかしな動作をするものがあるので, 4の倍数に切り上げる
  m_max_width = ((m_max_width + 3) >> 2) << 2;
  m_max_height = ((m_max_height + 3) >> 2) << 2;

  spdlog::debug("m_max_width x m_max_height: {} x {}", m_max_width,
                m_max_height);

  m_black_yuv_image = create_black_yuv_image(m_max_width, m_max_height);
}  // namespace hisui::video

BasicSequencer::~BasicSequencer() {
  delete m_black_yuv_image;
}

void BasicSequencer::getYUVs(std::vector<const YUVImage*>* yuvs,
                             const std::uint64_t timestamp) {
  size_t i = 0;
  for (const auto& p : m_sequence) {
    bool found = false;
    for (const auto& s : *p.second) {
      if (s.second.isIn(timestamp)) {
        (*yuvs)[i] = s.first->getYUV(s.second.getSubstructLower(timestamp));
        found = true;
        break;
      }
    }
    if (!found) {
      (*yuvs)[i] = m_black_yuv_image;
    }
    ++i;
  }
}

}  // namespace hisui::video
