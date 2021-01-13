#include <boost/test/unit_test.hpp>

#include "codec_def.h"

#include "video/h264.hpp"
#include "video/yuv.hpp"

BOOST_AUTO_TEST_SUITE(h264)

BOOST_AUTO_TEST_CASE(update_yuv_image_by_openh264_buffer_info_1) {
  hisui::video::YUVImage* yuv_image = new hisui::video::YUVImage(4, 2);
  ::SBufferInfo buffer_info;
  buffer_info.UsrData.sSystemBuffer.iWidth = 4;
  buffer_info.UsrData.sSystemBuffer.iHeight = 2;
  buffer_info.UsrData.sSystemBuffer.iStride[0] = 4;
  buffer_info.UsrData.sSystemBuffer.iStride[1] = 2;
  unsigned char buf0[] = {0, 0, 0, 0, 1, 1, 1, 1};
  unsigned char buf1[] = {2, 2};
  unsigned char buf2[] = {3, 3};
  buffer_info.pDst[0] = buf0;
  buffer_info.pDst[1] = buf1;
  buffer_info.pDst[2] = buf2;
  buffer_info.iBufferStatus = 1;

  update_yuv_image_by_openh264_buffer_info(yuv_image, buffer_info);

  BOOST_REQUIRE_EQUAL(yuv_image->getWidth(0), 4);
  BOOST_REQUIRE_EQUAL(yuv_image->getHeight(0), 2);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[0], yuv_image->yuv[0] + 8,
                                  buf0, buf0 + 8);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[1], yuv_image->yuv[1] + 2,
                                  buf1, buf1 + 2);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[2], yuv_image->yuv[2] + 2,
                                  buf2, buf2 + 2);
}

BOOST_AUTO_TEST_CASE(update_yuv_image_by_openh264_buffer_info_2) {
  hisui::video::YUVImage* yuv_image = new hisui::video::YUVImage(2, 2);
  ::SBufferInfo buffer_info;
  buffer_info.UsrData.sSystemBuffer.iWidth = 4;
  buffer_info.UsrData.sSystemBuffer.iHeight = 2;
  buffer_info.UsrData.sSystemBuffer.iStride[0] = 4;
  buffer_info.UsrData.sSystemBuffer.iStride[1] = 2;
  unsigned char buf0[] = {0, 0, 0, 0, 1, 1, 1, 1};
  unsigned char buf1[] = {2, 2};
  unsigned char buf2[] = {3, 3};
  buffer_info.pDst[0] = buf0;
  buffer_info.pDst[1] = buf1;
  buffer_info.pDst[2] = buf2;
  buffer_info.iBufferStatus = 1;

  update_yuv_image_by_openh264_buffer_info(yuv_image, buffer_info);

  BOOST_REQUIRE_EQUAL(yuv_image->getWidth(0), 4);
  BOOST_REQUIRE_EQUAL(yuv_image->getHeight(0), 2);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[0], yuv_image->yuv[0] + 8,
                                  buf0, buf0 + 8);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[1], yuv_image->yuv[1] + 2,
                                  buf1, buf1 + 2);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[2], yuv_image->yuv[2] + 2,
                                  buf2, buf2 + 2);
}

BOOST_AUTO_TEST_CASE(update_yuv_image_by_openh264_buffer_info_3) {
  hisui::video::YUVImage* yuv_image = new hisui::video::YUVImage(4, 2);
  ::SBufferInfo buffer_info;
  buffer_info.UsrData.sSystemBuffer.iWidth = 4;
  buffer_info.UsrData.sSystemBuffer.iHeight = 2;
  buffer_info.UsrData.sSystemBuffer.iStride[0] = 6;
  buffer_info.UsrData.sSystemBuffer.iStride[1] = 2;
  unsigned char buf0[] = {0, 0, 0, 0, 100, 100, 1, 1, 1, 1, 100, 100};
  unsigned char buf1[] = {2, 2};
  unsigned char buf2[] = {3, 3};
  buffer_info.pDst[0] = buf0;
  buffer_info.pDst[1] = buf1;
  buffer_info.pDst[2] = buf2;
  buffer_info.iBufferStatus = 1;

  update_yuv_image_by_openh264_buffer_info(yuv_image, buffer_info);

  BOOST_REQUIRE_EQUAL(yuv_image->getWidth(0), 4);
  BOOST_REQUIRE_EQUAL(yuv_image->getHeight(0), 2);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[0], yuv_image->yuv[0] + 4,
                                  buf0, buf0 + 4);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[0] + 4, yuv_image->yuv[0] + 8,
                                  buf0 + 6, buf0 + 10);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[1], yuv_image->yuv[1] + 2,
                                  buf1, buf1 + 2);
  BOOST_REQUIRE_EQUAL_COLLECTIONS(yuv_image->yuv[2], yuv_image->yuv[2] + 2,
                                  buf2, buf2 + 2);
}

BOOST_AUTO_TEST_SUITE_END()
