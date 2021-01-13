#pragma once

#include <cstddef>
#include <cstdint>

namespace hisui {

struct Frame {
  const std::uint64_t timestamp;
  std::uint8_t* data;
  const std::size_t data_size;
  const bool is_key;
};

}  // namespace hisui
