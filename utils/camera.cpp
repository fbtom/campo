///
/// @file main.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#include "camera.hpp"

namespace utils {

auto getCameraIDs() -> std::vector<int> {
  std::vector<int> ids;
  cv::VideoCapture cap;
  for (int i = 0; i < 10; ++i) {
    if (cap.open(i)) {
      ids.push_back(i);
      cap.release();
    }
  }

  return ids;
}

auto getValidCameraID(const std::vector<int> &camera_ids,
                      const std::optional<int> saved_id) -> int {
  if (camera_ids.empty()) {
    return -1;
  }

  if (saved_id.has_value()) {
    for (int id : camera_ids) {
      if (id == saved_id.value()) {
        return id;
      }
    }
  }

  return camera_ids[0];
}

void refreshCameraList(std::vector<CameraData> &container,
                       const std::vector<int> &new_camera_ids) {
  container.clear();
  for (const auto &id : new_camera_ids) {
    CameraData camera_data{};
    camera_data.id = id;
    camera_data.capture.open(id);

    if (camera_data.capture.isOpened()) {
      camera_data.is_available = true;
    } else {
      camera_data.capture.release();
      camera_data.is_available = false;
    }
    container.emplace_back(camera_data);
  }
}

std::vector<gui::CameraStream>
processCameraFrames(std::vector<utils::CameraData> &cameras) {
  auto camera_streams = std::vector<gui::CameraStream>{};
  for (auto &camera : cameras) {
    if (camera.is_available) {
      camera.capture.set(cv::CAP_PROP_FPS, 60);

      camera.capture >> camera.frame;

      if (!camera.frame.empty()) {
        camera.texture_id = utils::cvMatToTexture(camera.frame);
        camera_streams.push_back({static_cast<ImTextureID>(camera.texture_id),
                                  camera.frame.cols, camera.frame.rows,
                                  camera.id});
      }
    }
  }

  return camera_streams;
}

} // namespace utils
