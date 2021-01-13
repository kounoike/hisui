#pragma once

namespace hisui::muxer {

class Muxer {
 public:
  virtual ~Muxer() {}
  virtual void run() = 0;
};

}  // namespace hisui::muxer
