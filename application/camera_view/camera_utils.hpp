///
/// @file camera_utils.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "camera_types.hpp"
#include "imgui.h"

namespace application {
namespace camera_view {

/// @brief Fit an image with given aspect ratio into a box
/// @param box_size The size of the box
/// @param img_aspect_ratio The aspect ratio of the image
/// @param out_img_render_size Output parameter for the resulting image size
void fitImageInBox(const ImVec2 &box_size, float img_aspect_ratio,
                   ImVec2 &out_img_render_size);

/// @brief Center an item in its parent container
/// @param parent_size The size of the parent container
/// @param item_size The size of the item to center
void centerItemPosition(const ImVec2 &parent_size, const ImVec2 &item_size);

/// @brief Calculate the grid layout based on number of cameras
/// @param num_cameras The number of cameras to display
/// @return A Grid structure with rows and columns
Grid calculateGridLayout(int num_cameras);

/// @brief Calculate the size of each grid item based on available space
/// @param available_region_size The available region size
/// @param grid The grid layout
/// @return The calculated size for each grid item
ImVec2 calculateItemSize(const ImVec2 &available_region_size, const Grid &grid);

} // namespace camera_view
} // namespace application
