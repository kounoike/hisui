#pragma once

#include <string>

namespace hisui::version {

std::string get_libvpx_version();
std::string get_libwebm_version();
std::string get_openh264_version();
#ifdef USE_FDK_AAC
std::string get_fdkaac_aacenc_version();
#endif

std::string get_hisui_version();
std::string get_cppmp4_version();

}  // namespace hisui::version
