#include "muxer/simple_mp4_muxer.hpp"

#include <iosfwd>
#include <vector>

#include "metadata.hpp"
#include "shiguredo/mp4/track/soun.hpp"
#include "shiguredo/mp4/track/vide.hpp"
#include "shiguredo/mp4/writer/simple_writer.hpp"

namespace shiguredo::mp4::track {

class Track;

}

namespace hisui::muxer {

SimpleMP4Muxer::SimpleMP4Muxer(const hisui::Config& t_config,
                               const MP4MuxerParameters& params)
    : MP4Muxer(params), m_config(t_config) {
  m_duration = static_cast<float>(params.max_stop_time_offset);
  m_simple_writer = new shiguredo::mp4::writer::SimpleWriter(
      m_ofs, {.mvhd_timescale = 1000, .duration = m_duration});
}

void SimpleMP4Muxer::setUp() {
  initialize(m_config, m_simple_writer, m_duration);
}

SimpleMP4Muxer::~SimpleMP4Muxer() {
  delete m_simple_writer;
}

void SimpleMP4Muxer::run() {
  m_simple_writer->writeFtypBox();

  mux();

  if (m_vide_track) {
    m_simple_writer->appendTrakAndUdtaBoxInfo({m_soun_track, m_vide_track});
  } else {
    m_simple_writer->appendTrakAndUdtaBoxInfo({m_soun_track});
  }
  m_simple_writer->writeFreeBoxAndMdatHeader();
  m_simple_writer->writeMoovBox();
}

void SimpleMP4Muxer::cleanUp() {}

}  // namespace hisui::muxer
