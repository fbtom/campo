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

/// @brief Get a list of available camera IDs.
/// @return std::vector<int> of available camera IDs.
auto getCameraIDs() -> std::vector<int>;

/// @brief Choose a camera ID which shall be used after intialization.
/// @param camera_ids as a list of available camera IDs.
/// @param saved_id as the camera ID saved in the configuration file.
/// @return Matched camera ID.
auto getValidCameraID(const std::vector<int> &camera_ids,
                      const std::optional<int> saved_id) -> int;

/// @brief Update the list of available cameras.
/// @param container as a list of CameraData.
/// @param new_camera_ids as a list of available camera IDs.
void refreshCameraList(std::vector<CameraData> &container,
                       const std::vector<int> &new_camera_ids);

/// @brief Processes frames and generates a list of CameraStream.
/// @param cameras as a list of CameraData representing the cameras.
/// @return std::vector<gui::CameraStream> containing camera information.
std::vector<gui::CameraStream>
processCameraFrames(std::vector<utils::CameraData> &cameras);

} // namespace utils
