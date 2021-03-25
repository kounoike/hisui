#include "report/reporter.hpp"

#include <string>

#include "boost/json/serialize.hpp"
#include "version/version.hpp"

namespace hisui::report {

std::string Reporter::makeSuccessReport() {
  collectVersions();

  return boost::json::serialize(report);
}

void Reporter::collectVersions() {
  report["versions"] = {
      {"libvpx", version::get_libvpx_version()},
      {"libwebm", version::get_libwebm_version()},
      {"openh264", version::get_openh264_version()},
#ifdef USE_FDK_AAC
      {"fdk-aac AACENC", version::get_fdkaac_aacenc_version()},
#endif
      {"hisui", version::get_hisui_version()},
      {"cpp-mp4", version::get_cppmp4_version()},
  };
}

void Reporter::open() {
  if (!m_reporter) {
    m_reporter = new Reporter();
  }
}

bool Reporter::hasInstance() {
  return m_reporter != nullptr;
}

Reporter& Reporter::getInstance() {
  return *m_reporter;
}

void Reporter::close() {
  delete m_reporter;
  m_reporter = nullptr;
}

}  // namespace hisui::report

