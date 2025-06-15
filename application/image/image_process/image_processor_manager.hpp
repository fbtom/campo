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
      filters_.push_back(std::move(filter));
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

  bool HasActiveFilters() const { return !filters_.empty(); }

private:
  std::unique_ptr<ImageProcessor> base_processor_;
  std::vector<std::unique_ptr<decorator::FilterDecorator>> filters_;
};

} // namespace process
} // namespace image
