#pragma once

namespace hisui::muxer {

class WebMMuxer {
 public:
  virtual ~WebMMuxer() {}
  virtual void run() = 0;
};

}  // namespace hisui::muxer
