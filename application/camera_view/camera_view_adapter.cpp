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

std::vector<common::CameraStream> CameraViewAdapter::ConvertStreamFormat(
    const std::vector<common::CameraStream> &old_streams) {
  std::vector<common::CameraStream> new_streams;

  for (const auto &old_stream : old_streams) {
    new_streams.push_back({old_stream.texture_id, old_stream.width,
                           old_stream.height, old_stream.original_id});
  }

  return new_streams;
}

void CameraViewAdapter::SetCameraData(
    const std::vector<common::CameraStream> &streams) {
  auto converted_streams = ConvertStreamFormat(streams);
  controller_.SetCameraData(converted_streams);
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
