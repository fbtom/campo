///
/// @file camera_view_controller.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#include "camera_view_controller.hpp"

namespace application {
namespace camera_view {

CameraViewController::CameraViewController() : is_grid_view_(true) {}

void CameraViewController::SetCameraData(
    const std::vector<common::CameraStream> &streams) {
  camera_streams_ = streams;
  grid_view_.SetCameraData(streams);
  single_view_.SetCameraData(streams);
}

std::optional<int> CameraViewController::RenderView() {
  const ImVec2 available_region_size = ImGui::GetContentRegionAvail();

  if (is_grid_view_) {
    grid_view_.Render([this](int camera_index) {
      // When a camera is selected in grid view, switch to single view
      single_view_.SetSelectedCamera(camera_index);
      is_grid_view_ = false;
    });
    return std::nullopt;
  } else {
    // Render single camera view
    return single_view_.Render(available_region_size);
  }
}

bool CameraViewController::RenderReturnButton() {
  bool button_clicked = false;

  if (!is_grid_view_) {
    button_clicked = ImGui::Button("Return to Grid View", ImVec2(-1, 0));
    if (button_clicked) {
      is_grid_view_ = true;
      single_view_.SetSelectedCamera(-1); // Clear selection
    }
  }

  return button_clicked;
}

bool CameraViewController::IsGridView() const { return is_grid_view_; }

std::optional<int> CameraViewController::GetSelectedCameraId() const {
  if (!is_grid_view_) {
    return single_view_.GetSelectedCameraId();
  }
  return std::nullopt;
}

} // namespace camera_view
} // namespace application
