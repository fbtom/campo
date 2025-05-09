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
#include <deque>
#include <memory>

namespace image {
namespace process {

class ImageProcessorManager {
public:
  ImageProcessorManager()
      : processor_(std::make_unique<image::BaseImageProcessor>()) {}

  void processFrame(cv::Mat &frame) {
    if (processor_) {
      processor_->Process(frame);
    }
  }

  void SetProcessor(std::unique_ptr<ImageProcessor> processor) {
    processor_ = std::move(processor);
  }

  std::unique_ptr<ImageProcessor> ReleaseProcessor() {
    return std::move(processor_);
  }

  std::unique_ptr<ImageProcessor> UndoLast() {
    if (!processor_)
      return nullptr;

    decorator::FilterDecorator *outer_decorator =
        dynamic_cast<decorator::FilterDecorator *>(processor_.get());
    if (outer_decorator) {
      std::unique_ptr<ImageProcessor> previous_head =
          outer_decorator->ReleaseImage();
      std::unique_ptr<ImageProcessor> removed_filter = std::move(processor_);
      processor_ = std::move(previous_head);
      return removed_filter;
    }
    return nullptr;
  }

private:
  std::unique_ptr<ImageProcessor> processor_;
};

} // namespace process
} // namespace image
