///
/// @file camera_utils.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#include "camera_utils.hpp"

namespace application {
namespace camera_view {

void fitImageInBox(const ImVec2 &box_size, float img_aspect_ratio,
                   ImVec2 &out_img_render_size) {
  out_img_render_size.x = box_size.x;
  out_img_render_size.y = out_img_render_size.x / img_aspect_ratio;

  if (out_img_render_size.y > box_size.y) {
    out_img_render_size.y = box_size.y;
    out_img_render_size.x = out_img_render_size.y * img_aspect_ratio;
  }
}

void centerItemPosition(const ImVec2 &parent_size, const ImVec2 &item_size) {
  float offset_x = (parent_size.x - item_size.x) * 0.5f;
  float offset_y = (parent_size.y - item_size.y) * 0.5f;
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (offset_x > 0 ? offset_x : 0));
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (offset_y > 0 ? offset_y : 0));
}

Grid calculateGridLayout(int num_cameras) {
  Grid grid{0, 0};

  if (num_cameras == 1) {
    grid.rows_ = 1;
    grid.cols_ = 1;
  } else if (num_cameras == 2) {
    grid.rows_ = 2;
    grid.cols_ = 1;
  } else if (num_cameras == 3 || num_cameras == 4) {
    grid.rows_ = 2;
    grid.cols_ = 2;
  }

  return grid;
}

ImVec2 calculateItemSize(const ImVec2 &available_region_size,
                         const Grid &grid) {
  float panel_width = available_region_size.x;
  float panel_height = available_region_size.y;

  return ImVec2(
      (panel_width - (grid.cols_ - 1) * ImGui::GetStyle().ItemSpacing.x) /
          grid.cols_,
      (panel_height - (grid.rows_ - 1) * ImGui::GetStyle().ItemSpacing.y) /
          grid.rows_);
}

} // namespace camera_view
} // namespace application
