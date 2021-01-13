#pragma once

#include "muxer/audio_producer.hpp"

namespace hisui {

class Config;
class Metadata;

}  // namespace hisui

namespace hisui::muxer {

class FDKAACAudioProducer : public AudioProducer {
 public:
  FDKAACAudioProducer(const hisui::Config&, const hisui::Metadata& i);
};

}  // namespace hisui::muxer
