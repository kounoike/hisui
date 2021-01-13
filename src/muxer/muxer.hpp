#pragma once

namespace hisui::muxer {

class Muxer {
 public:
  virtual ~Muxer() {}
  virtual void setUp() = 0;
  virtual void run() = 0;
  virtual void cleanUp() = 0;
};

}  // namespace hisui::muxer
