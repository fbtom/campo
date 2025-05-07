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

} // namespace utils
