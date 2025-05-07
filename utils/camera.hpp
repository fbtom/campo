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
#include <GLFW/glfw3.h>
#include <imgui.h>

// Project headers
#include "gui/grid_display.hpp"
#include "utils/conversions.hpp"

namespace utils {

struct CameraData {
  int id;
  cv::VideoCapture capture;
  cv::Mat frame;
  GLuint texture_id;
  bool is_available{false};
};

struct AppContext {
  std::vector<CameraData> *cameras_ptr;
  int *current_id_ptr;
};

auto getCameraIDs() -> std::vector<int>;

auto getValidCameraID(const std::vector<int> &camera_ids,
                      const std::optional<int> saved_id) -> int;

void refreshCameraList(std::vector<CameraData> &container,
                       const std::vector<int> &new_camera_ids);

void processCameraFrames(
    std::vector<utils::CameraData> &cameras,
    std::vector<gui::CameraStream> &current_camera_streams);

} // namespace utils
