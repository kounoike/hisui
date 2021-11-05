#pragma once

#include <filesystem>
#include <string>

namespace hisui {

class Archive {
 public:
  Archive(const std::filesystem::path&,
          const std::string&,
          const double,
          const double);

  std::filesystem::path getPath() const;
  std::string getConnectionID() const;
  double getStartTimeOffset() const;
  double getStopTimeOffset() const;
  void adjustTimeOffsets(double);

  Archive& operator=(const Archive& other);

 private:
  std::filesystem::path m_path;
  std::string m_connection_id;
  double m_start_time_offset;
  double m_stop_time_offset;
};

}  // namespace hisui
