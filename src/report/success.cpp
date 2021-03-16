#include "report/success.hpp"

#include <string>

#include "boost/json/serialize.hpp"
#include "version/version.hpp"

namespace hisui::report {

std::string SuccessReporter::make() {
  getVersions();

  return boost::json::serialize(report);
}

void SuccessReporter::getVersions() {
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

}  // namespace hisui::report

