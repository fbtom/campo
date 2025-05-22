///
/// @file camera_view_adapter.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "camera_view_controller.hpp"
#include "common/camera_stream.hpp"
#include <optional>
#include <vector>

namespace application {
namespace camera_view {

class CameraViewAdapter {
private:
  CameraViewController controller_;

  std::vector<common::CameraStream>
  ConvertStreamFormat(const std::vector<common::CameraStream> &old_streams);

public:
  CameraViewAdapter();

  void SetCameraData(const std::vector<common::CameraStream> &streams);

  std::optional<int> GetSelectedCameraId() const;

  std::optional<int> RenderView();
  bool RenderReturnButton();
  bool IsGridView() const;
};

} // namespace camera_view
} // namespace application
