#include "audio/webm_source.hpp"

#include <bits/exception.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <cstdio>
#include <iterator>
#include <stdexcept>

#include "audio/decoder.hpp"
#include "audio/opus_decoder.hpp"
#include "constants.hpp"
#include "webm/input/audio_context.hpp"

namespace hisui::audio {

WebMSource::WebMSource(const std::string& file_path) {
  m_file = std::fopen(file_path.c_str(), "rb");
  if (m_file == nullptr) {
    throw std::runtime_error("Unable to open: " + file_path);
  }

  m_webm = new hisui::webm::input::AudioContext();
  if (!m_webm->init(m_file)) {
    spdlog::info(
        "AudioContext initialization failed. no audio track or unsupported "
        "codec: file_path='{}'",
        file_path);
    std::fclose(m_file);
    delete m_webm;
    m_webm = nullptr;
    return;
  }

  switch (m_webm->getCodec()) {
    case hisui::webm::input::AudioCodec::Opus:
      m_channels = m_webm->getChannels(),
      m_sampling_rate = static_cast<std::uint64_t>(m_webm->getSamplingRate());
      m_decoder = new OpusDecoder(m_channels);
      break;
    default:
      // 対応していない WebM の場合は {0, 0} を返す
      std::fclose(m_file);
      delete m_webm;
      m_webm = nullptr;
      spdlog::info("unsupported audio codec: file_path ='{}'", file_path);
      return;
  }
}

WebMSource::~WebMSource() {
  if (m_webm) {
    delete m_webm;
    std::fclose(m_file);
  }
  if (m_decoder) {
    delete m_decoder;
  }
}

std::pair<std::int16_t, std::int16_t> WebMSource::getSample(
    const std::uint64_t position) {
  if (!m_decoder) {
    return {0, 0};
  }
  if (position < m_current_position) {
    return {0, 0};
  }

  if (std::empty(m_data)) {
    // データが空だったら次のフレームを読んで, その後の m_decoder と m_current_position の値に応じた処理を行なう
    readFrame();
    return getSample(position);
  }

  if (m_channels == 1) {
    const std::int16_t d = m_data.front();
    m_data.pop();
    return {d, d};
  }

  const std::int16_t f = m_data.front();
  m_data.pop();
  const std::int16_t s = m_data.front();
  m_data.pop();
  return {f, s};
}

void WebMSource::readFrame() {
  if (m_webm->readFrame()) {
    m_current_position = static_cast<std::uint64_t>(m_webm->getTimestamp()) *
                         m_sampling_rate / hisui::Constants::NANO_SECOND;
    auto decoded =
        m_decoder->decode(m_webm->getBuffer(), m_webm->getBufferSize());
    if (decoded.second > 0) {
      for (std::size_t i = 0; i < decoded.second; ++i) {
        m_data.push(decoded.first[i]);
      }
    }
  } else {
    delete m_decoder;
    m_decoder = nullptr;
  }
}

}  // namespace hisui::audio
