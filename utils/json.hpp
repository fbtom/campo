///
/// @file json.hpp
/// @author fbtom
/// @brief
/// @date 2025-04-06
///
/// @copyright Copyright (c) 2025
///

#pragma once

// System headers
#include <cstdio>
#include <fstream>
#include <optional>
#include <vector>

// External headers
#include "nlohmann/json.hpp"
#include "opencv2/opencv.hpp"

constexpr char const *CameraID = "camera_id";
constexpr char const *Config = "config.json";

namespace utils {

/// @brief  Load the camera ID from a JSON configuration file.
/// @return std::optional<int> representing the camera ID. If the file does not
///         exist or the camera ID is not found, it returns std::nullopt.
auto loadCameraID() {
  std::optional<int> camera_id{std::nullopt};
  std::ifstream file{Config};

  if (file.is_open()) {
    nlohmann::json config{};

    file >> config;

    if (config.contains(CameraID)) {
      camera_id = config[CameraID].get<int>();
    }
  }

  return camera_id;
}

/// @brief Save the camera ID to a JSON configuration file.
/// @param camera_id The camera ID to save.
auto saveCameraID(const int camera_id) {
  nlohmann::json config{};
  config[CameraID] = camera_id;

  std::ofstream file(std::string{Config});

  if (file.is_open()) {
    file << config.dump(4);
  } else {
    // TODO: create logs with glog
    std::cerr << "INFO: Failed to save camera id.\n";
  }
}

} // namespace utils
