#pragma once

#include <codec/api/svc/codec_def.h>

namespace hisui::video {

class YUVImage;

void update_yuv_image_by_openh264_buffer_info(YUVImage*, const ::SBufferInfo&);

}  // namespace hisui::video
