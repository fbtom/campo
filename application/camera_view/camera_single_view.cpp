///
/// @file camera_single_view.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#include "camera_single_view.hpp"

namespace application {
namespace camera_view {

CameraSingleView::CameraSingleView()
    : selected_camera_index_(std::nullopt), actual_camera_count_(0) {}

void CameraSingleView::SetCameraData(
    const std::vector<common::CameraStream> &streams) {
  camera_streams_ = streams;
  actual_camera_count_ = streams.size();

  if (selected_camera_index_.has_value() &&
      selected_camera_index_.value() >= actual_camera_count_) {
    selected_camera_index_ = std::nullopt;
  }
}

void CameraSingleView::SetSelectedCamera(int index) {
  if (index >= 0 && index < actual_camera_count_) {
    selected_camera_index_ = index;
  } else {
    selected_camera_index_ = std::nullopt;
  }
}

std::optional<int>
CameraSingleView::Render(const ImVec2 &available_region_size,
                         image::region::RegionSelector *region_selector) {
  if (!selected_camera_index_.has_value() ||
      selected_camera_index_.value() >= actual_camera_count_) {
    return std::nullopt;
  }

  const auto &feed = camera_streams_[selected_camera_index_.value()];

  ImGui::BeginChild("SingleCameraView", available_region_size, true,
                    ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse);

  ImVec2 child_content_size = ImGui::GetContentRegionAvail();

  if (feed.texture_id && feed.width > 0 && feed.height > 0) {
    float aspect_ratio = static_cast<float>(feed.width) / feed.height;
    ImVec2 img_render_size;
    fitImageInBox(child_content_size, aspect_ratio, img_render_size);
    centerItemPosition(child_content_size, img_render_size);

    ImVec2 image_pos = ImGui::GetCursorScreenPos();
    ImGui::Image(feed.texture_id, img_render_size);

    if (region_selector && region_selector->GetMode() ==
                               image::region::FilterMode::kPartialRegion) {
      bool is_hovered = ImGui::IsItemHovered();

      if (is_hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        ImVec2 relative_pos =
            ImVec2(mouse_pos.x - image_pos.x, mouse_pos.y - image_pos.y);
        region_selector->Select(relative_pos, ImVec2(feed.width, feed.height),
                                img_render_size);
      }

      if (region_selector->IsSelected() &&
          ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        ImVec2 relative_pos =
            ImVec2(mouse_pos.x - image_pos.x, mouse_pos.y - image_pos.y);
        region_selector->Update(relative_pos);
      }

      if (region_selector->IsSelected() &&
          ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        region_selector->Finish();
      }

      region_selector->RenderSelectionOverlay(image_pos, img_render_size);
    }

    ImGui::SetCursorPos(ImVec2(5, 5));
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 0.8f), "Camera %d - %dx%d",
                       feed.original_id, feed.width, feed.height);
  } else {
    ImGui::Text("Camera %d: Feed N/A", feed.original_id);
  }

  ImGui::EndChild();

  return feed.original_id;
}

std::optional<int> CameraSingleView::GetSelectedCameraIndex() const {
  return selected_camera_index_;
}

std::optional<int> CameraSingleView::GetSelectedCameraId() const {
  if (selected_camera_index_.has_value() &&
      selected_camera_index_.value() < actual_camera_count_) {
    return camera_streams_[selected_camera_index_.value()].original_id;
  }
  return std::nullopt;
}

} // namespace camera_view
} // namespace application
