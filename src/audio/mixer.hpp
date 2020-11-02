#pragma once

#include <cstdint>

namespace hisui::audio {

std::int16_t mix_sample_simple(std::int16_t, std::int16_t);

std::int16_t mix_sample_vttoth(std::int16_t, std::int16_t);

}  // namespace hisui::audio
