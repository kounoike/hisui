#pragma once

#include <opus.h>
#include <opus_types.h>

#include <array>
#include <cstdint>

#include "constants.hpp"

namespace hisui::audio {

struct CreateOpusPrivateDataParameters {
  const ::opus_int32 skip;
  const std::uint32_t sample_rate = hisui::Constants::PCM_SAMPLE_RATE;
};

std::array<std::uint8_t, 19> create_opus_private_data(
    const CreateOpusPrivateDataParameters&);

struct CreateOpusEncoderParameters {
  const std::uint32_t sample_rate = hisui::Constants::PCM_SAMPLE_RATE;
  const int channels = 2;
  const std::uint32_t bit_rate = hisui::Constants::OPUS_DEFAULT_BIT_RATE;
};

::OpusEncoder* create_opus_encoder(const CreateOpusEncoderParameters&);

struct CreateOpusDecoderParameters {
  const std::uint32_t sample_rate = hisui::Constants::PCM_SAMPLE_RATE;
  const int channels = 2;
};

::OpusDecoder* create_opus_decoder(const CreateOpusDecoderParameters&);

}  // namespace hisui::audio
