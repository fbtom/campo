///
/// @file camera_grid_view.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#include "camera_grid_view.hpp"
#include <algorithm>

namespace application {
namespace camera_view {

CameraGridView::CameraGridView() : current_page_(0), actual_camera_count_(0) {}

void CameraGridView::SetCameraData(
    const std::vector<common::CameraStream> &streams) {
  camera_streams_ = streams;
  actual_camera_count_ = streams.size();
  current_page_ = 0;
}

void CameraGridView::RenderNoCameraAvailable(const ImVec2 &region_size) {
  const char *text = "No camera available";
  ImVec2 text_size = ImGui::CalcTextSize(text);
  centerItemPosition(region_size, text_size);
  ImGui::TextUnformatted(text);
}

void CameraGridView::RenderCameraFeeds(
    int start_index, int end_index, const Grid &grid, const ImVec2 &item_size,
    std::function<void(int)> on_camera_selected) {
  for (auto i = start_index; i < end_index; ++i) {
    ImGui::BeginChild(
        ("CameraFeed" + std::to_string(i)).c_str(), item_size, true,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    const auto &feed = camera_streams_[i];
    ImVec2 child_content_size = ImGui::GetContentRegionAvail();

    if (feed.texture_id && feed.width > 0 && feed.height > 0) {
      float aspect_ratio = static_cast<float>(feed.width) / feed.height;
      ImVec2 img_render_size;
      fitImageInBox(child_content_size, aspect_ratio, img_render_size);
      centerItemPosition(child_content_size, img_render_size);

      ImGui::Image(feed.texture_id, img_render_size);
      if (ImGui::IsItemClicked()) {
        on_camera_selected(i);
      }
    } else {
      ImGui::Text("Camera %d: Feed N/A", feed.original_id);
    }
    ImGui::EndChild();
    if ((i - start_index + 1) % grid.cols_ != 0 && i < end_index - 1) {
      ImGui::SameLine();
    }
  }
}

void CameraGridView::RenderPageControls() {
  ImGui::Text("Page: %d / %d", current_page_ + 1,
              (actual_camera_count_ + kMaxPageCamera - 1) / kMaxPageCamera);

  if (current_page_ > 0 && ImGui::Button("Previous")) {
    current_page_--;
  }
  ImGui::SameLine();
  if (current_page_ <
      (actual_camera_count_ + kMaxPageCamera - 1) / kMaxPageCamera - 1) {
    if (ImGui::Button("Next")) {
      current_page_++;
    }
  }
}

void CameraGridView::Render(std::function<void(int)> on_camera_selected) {
  const ImVec2 available_region_size = ImGui::GetContentRegionAvail();

  if (actual_camera_count_ == 0) {
    RenderNoCameraAvailable(available_region_size);
    return;
  }

  const auto grid = calculateGridLayout(actual_camera_count_);
  const auto start_index = current_page_ * kMaxPageCamera;
  const auto end_index =
      std::min(start_index + kMaxPageCamera, actual_camera_count_);
  const auto item_size = calculateItemSize(available_region_size, grid);

  RenderCameraFeeds(start_index, end_index, grid, item_size,
                    on_camera_selected);

  if (actual_camera_count_ > kMaxPageCamera) {
    RenderPageControls();
  }
}

int CameraGridView::GetCurrentPage() const { return current_page_; }

} // namespace camera_view
} // namespace application
