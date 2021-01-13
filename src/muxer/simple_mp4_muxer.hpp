#pragma once

#include "muxer/mp4_muxer.hpp"

namespace hisui {

class Config;
class Metadata;

}  // namespace hisui

namespace shiguredo::mp4::writer {

class SimpleWriter;

}

namespace hisui::muxer {

class SimpleMP4Muxer : public MP4Muxer {
 public:
  SimpleMP4Muxer(const hisui::Config&, const hisui::Metadata&);
  ~SimpleMP4Muxer();
  void run();

 private:
  shiguredo::mp4::writer::SimpleWriter* m_simple_writer;
};

}  // namespace hisui::muxer
