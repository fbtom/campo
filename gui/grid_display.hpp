#pragma once

#include "gui_utils.hpp"
#include "imgui.h"
#include <algorithm>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace gui {
constexpr const int kMaxPageCamera{4};

struct CameraStream {
  ImTextureID textureId;
  int width;
  int height;
  int original_id;
};

class GridDisplay {
private:
  struct Grid {
    int rows;
    int cols;
  };
  int actual_camera_count_;
  std::vector<CameraStream> all_camera_streams_;
  int current_page_;

  void fitImageInBox(const ImVec2 &box_size, float img_aspect_ratio,
                     ImVec2 &out_img_render_size) {
    out_img_render_size.x = box_size.x;
    out_img_render_size.y = out_img_render_size.x / img_aspect_ratio;

    if (out_img_render_size.y > box_size.y) {
      out_img_render_size.y = box_size.y;
      out_img_render_size.x = out_img_render_size.y * img_aspect_ratio;
    }
  }

  void centerCurrentItemPos(const ImVec2 &parent_size,
                            const ImVec2 &item_size) {
    float offset_x = (parent_size.x - item_size.x) * 0.5f;
    float offset_y = (parent_size.y - item_size.y) * 0.5f;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                         (offset_x > 0 ? offset_x : 0));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                         (offset_y > 0 ? offset_y : 0));
  }

  std::optional<int> renderNoCameraAvailable(const ImVec2 &region_size) {
    const char *text = "No camera available";
    ImVec2 text_size = ImGui::CalcTextSize(text);
    centerCurrentItemPos(region_size, text_size);
    ImGui::TextUnformatted(text);
    return std::nullopt;
  }

  Grid getGridDisplaySize(int num_cameras) {
    Grid grid{0, 0};

    if (num_cameras == 1) {
      grid.rows = 1;
      grid.cols = 1;
    } else if (num_cameras == 2) {
      grid.rows = 2;
      grid.cols = 1;
    } else if (num_cameras == 3) {
      grid.rows = 2;
      grid.cols = 2;
    } else if (num_cameras == 4) {
      grid.rows = 2;
      grid.cols = 2;
    }

    return grid;
  }

  ImVec2 calculateItemSize(const ImVec2 &available_region_size,
                           const Grid &grid) {
    float panel_width = available_region_size.x;
    float panel_height = available_region_size.y;

    return ImVec2(
        (panel_width - (grid.cols - 1) * ImGui::GetStyle().ItemSpacing.x) /
            grid.cols,
        (panel_height - (grid.rows - 1) * ImGui::GetStyle().ItemSpacing.y) /
            grid.rows);
  }

  void renderCameraFeed(int start_index, int end_index, const Grid &grid,
                        const ImVec2 &item_size) {
    for (auto i = start_index; i < end_index; ++i) {
      ImGui::BeginChild(
          ("CameraFeed" + std::to_string(i)).c_str(), item_size, true,
          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

      const auto &feed = all_camera_streams_[i];
      ImVec2 child_content_size = ImGui::GetContentRegionAvail();

      if (feed.textureId && feed.width > 0 && feed.height > 0) {
        float aspect_ratio = static_cast<float>(feed.width) / feed.height;
        ImVec2 img_render_size;
        fitImageInBox(child_content_size, aspect_ratio, img_render_size);
        centerCurrentItemPos(child_content_size, img_render_size);
        ImGui::Image(feed.textureId, img_render_size);
      } else {
        ImGui::Text("Camera %d: Feed N/A", feed.original_id);
      }
      ImGui::EndChild();
      if ((i - start_index + 1) % grid.cols != 0 && i < end_index - 1) {
        ImGui::SameLine();
      }
    }
  }

  void renderPageControls() {
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

public:
  GridDisplay() : actual_camera_count_{0}, current_page_{0} {}

  void setCameraData(const std::vector<CameraStream> &streams) {
    actual_camera_count_ = streams.size();
    all_camera_streams_ = streams;
    current_page_ = 0;
  }

  std::optional<int> renderGrid() {
    const ImVec2 available_region_size = ImGui::GetContentRegionAvail();

    if (actual_camera_count_ == 0) {
      return renderNoCameraAvailable(available_region_size);
    }

    const auto grid = getGridDisplaySize(actual_camera_count_);
    const auto start_index = current_page_ * kMaxPageCamera;
    const auto end_index =
        std::min(start_index + kMaxPageCamera, actual_camera_count_);
    const auto item_size = calculateItemSize(available_region_size, grid);

    renderCameraFeed(start_index, end_index, grid, item_size);

    if (actual_camera_count_ > kMaxPageCamera) {
      renderPageControls();
    }

    return std::nullopt;
  }
};

} // namespace gui
