///
/// @file region_selector.hpp
/// @author fbtom
/// @brief
/// @date 2025-06-15
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "imgui.h"
#include "opencv2/opencv.hpp"
#include <algorithm>
#include <cmath>
#include <optional>

namespace image {
namespace region {

struct Region {
  cv::Rect rect;
  bool is_valid{false};
  bool is_selected{false};
  ImVec2 start_pos;
  ImVec2 end_pos;
};

enum class FilterMode { kFullScreen, kPartialRegion };

class RegionSelector {
public:
  RegionSelector()
      : mode_(FilterMode::kFullScreen), selection_{}, image_size_{},
        display_size_{} {}

  void ToggleMode() {
    if (mode_ == FilterMode::kFullScreen) {
      mode_ = FilterMode::kPartialRegion;
    } else {
      mode_ = FilterMode::kFullScreen;
      ClearSelection();
    }
  }

  FilterMode GetMode() const { return mode_; }

  void Select(const ImVec2 &pos, const ImVec2 &image_size,
              const ImVec2 &display_size) {
    if (mode_ != FilterMode::kPartialRegion) {
      return;
    }

    image_size_ = image_size;
    display_size_ = display_size;
    selection_.is_selected = true;
    selection_.is_valid = false;
    selection_.start_pos = pos;
    selection_.end_pos = pos;
  }

  void Update(const ImVec2 &pos) {
    if (!IsSelected()) {
      return;
    }

    selection_.end_pos = pos;
  }

  void Finish() {
    if (!IsSelected()) {
      return;
    }

    selection_.is_selected = false;

    cv::Point2f start_img = ScreenToImageCoords(selection_.start_pos);
    cv::Point2f end_img = ScreenToImageCoords(selection_.end_pos);

    float min_x = std::min(start_img.x, end_img.x);
    float max_x = std::max(start_img.x, end_img.x);
    float min_y = std::min(start_img.y, end_img.y);
    float max_y = std::max(start_img.y, end_img.y);

    if ((max_x - min_x) >= 10 && (max_y - min_y) >= 10) {
      selection_.rect = cv::Rect(
          static_cast<int>(min_x), static_cast<int>(min_y),
          static_cast<int>(max_x - min_x), static_cast<int>(max_y - min_y));

      selection_.rect.x = std::max(0, selection_.rect.x);
      selection_.rect.y = std::max(0, selection_.rect.y);
      selection_.rect.width =
          std::min(static_cast<int>(image_size_.x) - selection_.rect.x,
                   selection_.rect.width);
      selection_.rect.height =
          std::min(static_cast<int>(image_size_.y) - selection_.rect.y,
                   selection_.rect.height);

      selection_.is_valid =
          (selection_.rect.width > 0 && selection_.rect.height > 0);
    } else {
      selection_.is_valid = false;
    }
  }

  void Cancel() {
    selection_.is_selected = false;
    selection_.is_valid = false;
  }

  std::optional<cv::Rect> GetRegion() const {
    if (mode_ == FilterMode::kFullScreen || !isValid()) {
      return std::nullopt;
    }
    return selection_.rect;
  }

  bool IsSelected() const { return selection_.is_selected; }

  void RenderSelectionOverlay(const ImVec2 &image_pos,
                              const ImVec2 &image_size) {
    if (mode_ != FilterMode::kPartialRegion) {
      return;
    }

    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    if (IsSelected()) {
      ImVec2 min_pos =
          ImVec2(std::min(selection_.start_pos.x, selection_.end_pos.x),
                 std::min(selection_.start_pos.y, selection_.end_pos.y));
      ImVec2 max_pos =
          ImVec2(std::max(selection_.start_pos.x, selection_.end_pos.x),
                 std::max(selection_.start_pos.y, selection_.end_pos.y));

      min_pos.x += image_pos.x;
      min_pos.y += image_pos.y;
      max_pos.x += image_pos.x;
      max_pos.y += image_pos.y;

      draw_list->AddRect(min_pos, max_pos, IM_COL32(255, 255, 0, 180), 0.0f, 0,
                         2.0f);
      draw_list->AddRectFilled(min_pos, max_pos, IM_COL32(255, 255, 0, 30));
    } else if (isValid()) {
      ImVec2 rect_start = ImageToScreenCoords(
          cv::Point2f(selection_.rect.x, selection_.rect.y));
      ImVec2 rect_end = ImageToScreenCoords(
          cv::Point2f(selection_.rect.x + selection_.rect.width,
                      selection_.rect.y + selection_.rect.height));

      rect_start.x += image_pos.x;
      rect_start.y += image_pos.y;
      rect_end.x += image_pos.x;
      rect_end.y += image_pos.y;

      draw_list->AddRect(rect_start, rect_end, IM_COL32(0, 255, 0, 200), 0.0f,
                         0, 2.0f);
      draw_list->AddRectFilled(rect_start, rect_end, IM_COL32(0, 255, 0, 40));
    }
  }

  void ClearSelection() {
    selection_.is_valid = false;
    selection_.is_selected = false;
  }

  bool HasValidSelection() const {
    return mode_ == FilterMode::kPartialRegion && isValid();
  }

private:
  FilterMode mode_;
  Region selection_;
  ImVec2 image_size_;
  ImVec2 display_size_;

  cv::Point2f ScreenToImageCoords(const ImVec2 &screen_pos) {
    float scale_x = image_size_.x / display_size_.x;
    float scale_y = image_size_.y / display_size_.y;

    return cv::Point2f(screen_pos.x * scale_x, screen_pos.y * scale_y);
  }

  ImVec2 ImageToScreenCoords(const cv::Point2f &image_pos) {
    float scale_x = display_size_.x / image_size_.x;
    float scale_y = display_size_.y / image_size_.y;

    return ImVec2(image_pos.x * scale_x, image_pos.y * scale_y);
  }

  bool isValid() const { return selection_.is_valid; }
};

} // namespace region
} // namespace image
