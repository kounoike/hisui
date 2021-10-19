#include "layout/audio_source.hpp"

#include <string>

#include "audio/webm_source.hpp"

namespace hisui::layout {

AudioSource::AudioSource(const SourceParameters& params) : Source(params) {
  source = std::make_shared<hisui::audio::WebMSource>(file_path.string());
}

}  // namespace hisui::layout

