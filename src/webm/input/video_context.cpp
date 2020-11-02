#include "webm/input/video_context.hpp"

#include <bits/exception.h>

#include <cstring>

#include "mkvparser/mkvparser.h"
#include "spdlog/fmt/fmt.h"
#include "spdlog/spdlog.h"

#include "constants.hpp"

namespace hisui::webm::input {

VideoContext::VideoContext() {}

VideoContext::~VideoContext() {
  reset();
}

void VideoContext::reset() {
  Context::reset();
  m_fourcc = 0;
}

bool VideoContext::init(std::FILE* file) {
  initReaderAndSegment(file);

  const mkvparser::Tracks* const tracks = m_segment->GetTracks();
  const mkvparser::VideoTrack* video_track = nullptr;
  for (std::uint64_t i = 0, m = tracks->GetTracksCount(); i < m; ++i) {
    const mkvparser::Track* const track = tracks->GetTrackByIndex(i);
    if (track != nullptr && track->GetType() == mkvparser::Track::kVideo) {
      video_track = static_cast<const mkvparser::VideoTrack*>(track);
      m_track_index = static_cast<int>(track->GetNumber());
      break;
    }
  }

  if (video_track == nullptr || video_track->GetCodecId() == nullptr) {
    spdlog::info("video track not found");
    reset();
    return false;
  }

  if (!std::strncmp(video_track->GetCodecId(), "V_VP8", 5)) {
    m_fourcc = hisui::Constants::VP8_FOURCC;
  } else if (!std::strncmp(video_track->GetCodecId(), "V_VP9", 5)) {
    m_fourcc = hisui::Constants::VP9_FOURCC;
  } else if (!std::strncmp(video_track->GetCodecId(), "V_MPEG4/ISO/AVC", 15) &&
             !std::strncmp(video_track->GetCodecNameAsUTF8(), "H.264", 5)) {
    m_fourcc = hisui::Constants::H264_FOURCC;
  } else {
    if (video_track->GetCodecNameAsUTF8() == nullptr) {
      spdlog::info("unsuppoted codec: codec_id={}", video_track->GetCodecId());
    } else {
      spdlog::info("unsuppoted codec: codec_id={}, codec_name={}",
                   video_track->GetCodecId(),
                   video_track->GetCodecNameAsUTF8());
    }
    reset();
    return false;
  }

  m_width = static_cast<std::uint32_t>(video_track->GetWidth());
  m_height = static_cast<std::uint32_t>(video_track->GetHeight());

  m_cluster = m_segment->GetFirst();

  return true;
}

std::uint32_t VideoContext::getFourcc() const {
  return m_fourcc;
}

std::uint32_t VideoContext::getWidth() const {
  return m_width;
}

std::uint32_t VideoContext::getHeight() const {
  return m_height;
}

}  // namespace hisui::webm::input
