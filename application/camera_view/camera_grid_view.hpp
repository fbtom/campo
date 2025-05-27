///
/// @file camera_grid_view.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "camera_types.hpp"
#include "camera_utils.hpp"
#include "imgui.h"
#include <functional>
#include <optional>
#include <vector>

namespace application {
namespace camera_view {

class CameraGridView {
private:
  std::vector<common::CameraStream> camera_streams_;
  int current_page_;
  int actual_camera_count_;

  void RenderNoCameraAvailable(const ImVec2 &region_size);
  void RenderCameraFeeds(int start_index, int end_index, const Grid &grid,
                         const ImVec2 &item_size,
                         std::function<void(int)> on_camera_selected);

  void RenderPageControls();

public:
  CameraGridView();

  void SetCameraData(const std::vector<common::CameraStream> &streams);

  void Render(std::function<void(int)> on_camera_selected);
  int GetCurrentPage() const;
};

} // namespace camera_view
} // namespace application
