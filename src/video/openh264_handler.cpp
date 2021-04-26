#include "video/openh264_handler.hpp"

#include <dlfcn.h>

#include <stdexcept>

namespace hisui::video {

void OpenH264Handler::open(const std::string& openh264_path) {
  if (!m_handler) {
    m_handler = new OpenH264Handler(openh264_path);
  }
}

bool OpenH264Handler::hasInstance() {
  return m_handler != nullptr;
}

OpenH264Handler& OpenH264Handler::getInstance() {
  return *m_handler;
}

void OpenH264Handler::close() {
  delete m_handler;
  m_handler = nullptr;
}

OpenH264Handler::OpenH264Handler(const std::string& openh264_path) {
  if (m_openh264_handle != nullptr) {
    return;
  }

  m_openh264_handle = ::dlopen(openh264_path.c_str(), RTLD_LAZY);
  if (m_openh264_handle == nullptr) {
    throw std::runtime_error("dlopen(openh264) failed");
  }
  createDecoder = reinterpret_cast<CreateDecoderFunc>(
      ::dlsym(m_openh264_handle, "WelsCreateDecoder"));
  if (createDecoder == nullptr) {
    ::dlclose(m_openh264_handle);
    throw std::runtime_error("dlsym(WelsCreateDecoder) failed");
  }
  destroyDecoder = reinterpret_cast<DestroyDecoderFunc>(
      ::dlsym(m_openh264_handle, "WelsDestroyDecoder"));
  if (destroyDecoder == nullptr) {
    ::dlclose(m_openh264_handle);
    throw std::runtime_error("dlsym(WelsDestroyDecoder) failed");
  }

  createEncoder = reinterpret_cast<CreateEncoderFunc>(
      ::dlsym(m_openh264_handle, "WelsCreateSVCEncoder"));
  if (createEncoder == nullptr) {
    ::dlclose(m_openh264_handle);
    throw std::runtime_error("dlsym(WelsCreateSVCEncoder) failed");
  }
  destroyEncoder = reinterpret_cast<DestroyEncoderFunc>(
      ::dlsym(m_openh264_handle, "WelsDestroySVCEncoder"));
  if (destroyEncoder == nullptr) {
    ::dlclose(m_openh264_handle);
    throw std::runtime_error("dlsym(WelsDestroySVCEncoder) failed");
  }
  getCodecVersion = reinterpret_cast<GetCodecVersoinFunc>(
      ::dlsym(m_openh264_handle, "WelsGetCodecVersion"));
  if (getCodecVersion == nullptr) {
    ::dlclose(m_openh264_handle);
    throw std::runtime_error("dlsym(WelsGetCodecVersion) failed");
  }
}

OpenH264Handler::~OpenH264Handler() {
  ::dlclose(m_openh264_handle);
}

}  // namespace hisui::video
