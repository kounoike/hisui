#pragma once

#include <opus.h>
#include <opus_types.h>

#include <array>
#include <cstdint>

namespace hisui::audio {

std::array<std::uint8_t, 19> create_opus_private_data(const ::opus_int32,
                                                      const std::uint32_t);

::OpusEncoder* create_opus_encoder(const std::uint32_t,
                                   const int,
                                   const std::uint32_t);

::OpusDecoder* create_opus_decoder(const std::uint32_t, const int);

}  // namespace hisui::audio
