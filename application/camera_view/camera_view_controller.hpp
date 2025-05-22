///
/// @file camera_view_controller.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "camera_grid_view.hpp"
#include "camera_single_view.hpp"
#include "camera_types.hpp"
#include <optional>
#include <vector>

namespace application {
namespace camera_view {

class CameraViewController {
private:
  CameraGridView grid_view_;
  CameraSingleView single_view_;
  bool is_grid_view_;
  std::vector<common::CameraStream> camera_streams_;

public:
  CameraViewController();

  void SetCameraData(const std::vector<common::CameraStream> &streams);

  std::optional<int> GetSelectedCameraId() const;

  std::optional<int> RenderView();
  bool RenderReturnButton();
  bool IsGridView() const;
};

} // namespace camera_view
} // namespace application
