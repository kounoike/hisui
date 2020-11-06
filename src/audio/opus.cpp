#include "audio/opus.hpp"

#include <fmt/core.h>
#include <opus_defines.h>

#include <stdexcept>

namespace hisui::audio {

std::array<std::uint8_t, 19> create_opus_private_data(
    const ::opus_int32 skip,
    const std::uint32_t sample_rate) {
  // https://wiki.xiph.org/OggOpus
  std::array<std::uint8_t, 19> data;
  data[0] = 'O';
  data[1] = 'p';
  data[2] = 'u';
  data[3] = 's';
  data[4] = 'H';
  data[5] = 'e';
  data[6] = 'a';
  data[7] = 'd';
  data[8] = 1;  // version nunber
  data[9] = 2;  // channel count
  data[10] = skip & 0xff;
  data[11] = (skip >> 8) & 0xff;
  data[12] = sample_rate & 0xff;
  data[13] = (sample_rate >> 8) & 0xff;
  data[14] = (sample_rate >> 16) & 0xff;
  data[15] = (sample_rate >> 24) & 0xff;
  data[16] = 0;  // output gain
  data[17] = 0;  // output gain
  data[18] = 0;  // mapping family

  return data;
}

::OpusEncoder* create_opus_encoder(const std::uint32_t sample_rate,
                                   const int channels,
                                   const std::uint32_t bitrate) {
  int err;
  ::OpusEncoder* encoder =
      ::opus_encoder_create(static_cast<opus_int32>(sample_rate), channels,
                            OPUS_APPLICATION_AUDIO, &err);
  if (err < 0) {
    throw std::runtime_error(fmt::format(
        "opus_encoder_create() failed: error='{}'", ::opus_strerror(err)));
  }

  const int encoder_ctl_ret =
      ::opus_encoder_ctl(encoder, OPUS_SET_BITRATE(bitrate));
  if (encoder_ctl_ret < 0) {
    throw std::runtime_error(
        fmt::format("opus_encoder_ctl(BITRATE) failed: error='{}'",
                    ::opus_strerror(encoder_ctl_ret)));
  }

  return encoder;
}

::OpusDecoder* create_opus_decoder(const std::uint32_t sample_rate,
                                   const int channels) {
  int opus_err = 0;
  ::OpusDecoder* decoder = ::opus_decoder_create(
      static_cast<opus_int32>(sample_rate), channels, &opus_err);

  if (opus_err) {
    throw std::runtime_error(fmt::format(
        "failed opus_decoder_create(): error='{}'", ::opus_strerror(opus_err)));
  }
  return decoder;
}

}  // namespace hisui::audio
