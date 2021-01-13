#pragma once

#include "muxer/mp4_muxer.hpp"

namespace shiguredo::mp4::writer {

class FaststartWriter;

}

namespace hisui {

class Config;
class Metadata;

}  // namespace hisui

namespace hisui::muxer {

class FaststartMP4Muxer : public MP4Muxer {
 public:
  FaststartMP4Muxer(const hisui::Config&, const hisui::Metadata&);
  ~FaststartMP4Muxer();

  void run();

 private:
  shiguredo::mp4::writer::FaststartWriter* m_faststart_writer;
};

}  // namespace hisui::muxer
