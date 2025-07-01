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
#include <optional>
#include <vector>

// External headers
#include "nlohmann/json.hpp"
#include "opencv2/opencv.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>

// Project headers
#include "application/image/history/command_history.hpp"
#include "application/image/image_process/image_processor_manager.hpp"
#include "application/image/region/region_selector.hpp"
#include "common/camera_stream.hpp"
#include "utils/conversions.hpp"

namespace utils {

struct CameraData;

struct CameraData {
  int id;
  cv::VideoCapture capture;
  cv::Mat frame;
  GLuint texture_id;
  bool is_available{false};
  std::unique_ptr<image::process::ImageProcessorManager> processor_manager{
      nullptr};
  std::unique_ptr<image::history::CommandHistory> command_history{nullptr};
};

struct AppContext {
  std::vector<CameraData> *cameras_ptr;
  int *current_id_ptr;
  image::history::CommandHistory *command_history_ptr;
  image::process::ImageProcessorManager *image_processor_manager_ptr;
  image::region::RegionSelector *region_selector_ptr;
  int blur_intensity = 1;
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
/// @param selected_camera_id Optional ID of the selected camera to process
/// (when in single camera view).
/// @return std::vector<common::CameraStream> containing camera information.
std::vector<common::CameraStream>
processCameraFrames(std::vector<utils::CameraData> &cameras,
                    std::optional<int> selected_camera_id = std::nullopt);

} // namespace utils
