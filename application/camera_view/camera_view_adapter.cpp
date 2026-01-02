///
/// @file camera_view_adapter.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#include "camera_view_adapter.hpp"
#include "application/gui/grid_display.hpp"

namespace application {
namespace camera_view {

CameraViewAdapter::CameraViewAdapter() {}

void CameraViewAdapter::SetCameraData(
    const std::vector<common::CameraStream> &streams) {
  controller_.SetCameraData(streams);
}

std::optional<int>
CameraViewAdapter::RenderView(image::region::RegionSelector *region_selector) {
  return controller_.RenderView(region_selector);
}

bool CameraViewAdapter::RenderReturnButton() {
  return controller_.RenderReturnButton();
}

bool CameraViewAdapter::IsGridView() const { return controller_.IsGridView(); }

std::optional<int> CameraViewAdapter::GetSelectedCameraId() const {
  return controller_.GetSelectedCameraId();
}

} // namespace camera_view
} // namespace application
