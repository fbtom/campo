///
/// @file image_processor_manager.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/image_process/image_processor.hpp"
#include "opencv2/opencv.hpp"
#include <algorithm>
#include <memory>
#include <optional>
#include <vector>

namespace image {
namespace process {

class ImageProcessorManager {
public:
  ImageProcessorManager()
      : base_processor_(std::make_unique<image::BaseImageProcessor>()) {}

  void processFrame(cv::Mat &frame) {
    for (auto &filter : filters_) {
      if (filter) {
        filter->Process(frame);
      }
    }
  }

  void AddFilter(std::unique_ptr<decorator::FilterDecorator> filter) {
    if (filter) {
      if (default_region_.has_value()) {
        filter->SetProcessingRegion(default_region_);
      }
      filters_.push_back(std::move(filter));
    }
  }

  void AddFilter(std::unique_ptr<decorator::FilterDecorator> filter,
                 const std::optional<cv::Rect> &region) {
    if (filter) {
      filter->SetProcessingRegion(region);
      filters_.push_back(std::move(filter));
    }
  }

  void AddFilterAtIndex(std::unique_ptr<decorator::FilterDecorator> filter,
                        size_t index) {
    if (filter) {
      if (default_region_.has_value()) {
        filter->SetProcessingRegion(default_region_);
      }
      if (index >= filters_.size()) {
        filters_.push_back(std::move(filter));
      } else {
        filters_.insert(filters_.begin() + index, std::move(filter));
      }
    }
  }

  void SetDefaultRegion(const std::optional<cv::Rect> &region) {
    default_region_ = region;
  }

  void SetProcessingRegion(const std::optional<cv::Rect> &region) {
    for (auto &filter : filters_) {
      if (filter) {
        filter->SetProcessingRegion(region);
      }
    }
  }

  std::unique_ptr<decorator::FilterDecorator> Undo() {
    if (filters_.empty()) {
      return nullptr;
    }

    std::unique_ptr<decorator::FilterDecorator> removed_filter =
        std::move(filters_.back());
    filters_.pop_back();

    return removed_filter;
  }

  std::vector<std::string> GetActiveFilters() const {
    std::vector<std::string> filter_names;

    std::for_each(filters_.begin(), filters_.end(),
                  [&filter_names](const auto &filter) {
                    if (filter) {
                      filter_names.push_back(filter->GetFilterName());
                    }
                  });

    return filter_names;
  }

  std::unique_ptr<decorator::FilterDecorator> RemoveFilter(size_t index) {
    if (index >= filters_.size()) {
      return nullptr;
    }

    std::unique_ptr<decorator::FilterDecorator> removed_filter =
        std::move(filters_[index]);
    filters_.erase(filters_.begin() + index);

    return removed_filter;
  }

  void ToggleFilter(size_t index) {
    if (index < filters_.size() && filters_[index]) {
      bool current_state = filters_[index]->IsEnabled();
      filters_[index]->SetEnabled(!current_state);
    }
  }

  bool IsFilterEnabled(size_t index) const {
    if (index < filters_.size() && filters_[index]) {
      return filters_[index]->IsEnabled();
    }
    return false;
  }

  size_t GetFilterCount() const { return filters_.size(); }

  bool HasActiveFilters() const { return !filters_.empty(); }

private:
  std::unique_ptr<ImageProcessor> base_processor_;
  std::vector<std::unique_ptr<decorator::FilterDecorator>> filters_;
  std::optional<cv::Rect> default_region_;
};

} // namespace process
} // namespace image
