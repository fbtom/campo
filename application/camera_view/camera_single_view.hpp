///
/// @file camera_single_view.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "camera_types.hpp"
#include "camera_utils.hpp"
#include <optional>
#include <vector>

namespace application {
namespace camera_view {

class CameraSingleView {
private:
  std::vector<common::CameraStream> camera_streams_;
  std::optional<int> selected_camera_index_;
  int actual_camera_count_;

public:
  CameraSingleView();

  void SetCameraData(const std::vector<common::CameraStream> &streams);
  void SetSelectedCamera(int index);
  
  std::optional<int> Render(const ImVec2 &available_region_size);
  std::optional<int> GetSelectedCameraIndex() const;
  std::optional<int> GetSelectedCameraId() const;
};

} // namespace camera_view
} // namespace application
