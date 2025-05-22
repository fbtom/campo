///
/// @file camera_types.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "common/camera_stream.hpp"
#include <optional>
#include <vector>

namespace application {
namespace camera_view {

struct Grid {
  int rows_;
  int cols_;
};

constexpr const int kMaxPageCamera{4};

} // namespace camera_view
} // namespace application
