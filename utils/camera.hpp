///
/// @file camera_utils.hpp
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
#include <vector>

// External headers
#include "nlohmann/json.hpp"
#include "opencv2/opencv.hpp"

namespace utils {
auto getCameraIDs() {
  FILE *system_stderr = stderr;
#ifdef _WIN32
  FILE *null_stream = freopen("NUL", "w", stderr);
#else
  FILE *null_stream = freopen("/dev/null", "w", stderr);
#endif

  std::vector<int> camera_ids{};
  size_t max_camera_id = 50;
  for (size_t i = 0; i < max_camera_id; ++i) {
    cv::VideoCapture cam(i);
    if (cam.isOpened()) {
      camera_ids.push_back(i);
      cam.release();
    }
  }

  fflush(stderr);
  stderr = system_stderr;

  return camera_ids;
}

auto getValidCameraID(const std::vector<int> &camera_ids,
                      const std::optional<int> saved_id) {
  if (camera_ids.empty()) {
    std::cerr << "No camera has been found.\n";
    return -1;
  }

  if (saved_id.has_value()) {
    const auto id = saved_id.value();
    if (std::find(camera_ids.begin(), camera_ids.end(), id) !=
        camera_ids.end()) {
      return id;
    }
  }

  return camera_ids.front();
}

} // namespace utils
