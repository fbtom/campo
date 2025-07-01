///
/// @file grid_display.hpp
/// @author fbtom
/// @brief Grid display for camera view - Adapter to new camera view module
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/camera_view/camera_view_adapter.hpp"
#include "application/image/region/region_selector.hpp"
#include "common/camera_stream.hpp"
#include "gui_utils.hpp"
#include "imgui.h"
#include <algorithm>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace gui {

class GridDisplay {
private:
  application::camera_view::CameraViewAdapter adapter_;

public:
  GridDisplay() {}

  void SetCameraData(const std::vector<common::CameraStream> &streams) {
    adapter_.SetCameraData(streams);
  }

  bool RenderReturnButton() { return adapter_.RenderReturnButton(); }

  bool IsGridView() const { return adapter_.IsGridView(); }

  std::optional<int> GetSelectedCameraId() const {
    return adapter_.GetSelectedCameraId();
  }

  std::optional<int>
  RenderGrid(image::region::RegionSelector *region_selector = nullptr) {
    return adapter_.RenderView(region_selector);
  }
};

} // namespace gui
