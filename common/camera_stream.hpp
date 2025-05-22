///
/// @file camera_stream.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-22
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "imgui.h"

namespace common {

struct CameraStream {
  ImTextureID texture_id;
  int width;
  int height;
  int original_id;
};

} // namespace common
