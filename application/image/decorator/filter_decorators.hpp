///
/// @file filter_decorators.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/image_process/image_processor.hpp"
#include "opencv2/opencv.hpp"
#include <memory>

namespace image {
namespace decorator {

class FilterDecorator : public ImageProcessor {
protected:
  std::unique_ptr<ImageProcessor> image_;

public:
  FilterDecorator(std::unique_ptr<ImageProcessor> processor)
      : image_(std::move(processor)) {}

  void SetImage(std::unique_ptr<ImageProcessor> processor) {
    image_ = std::move(processor);
  }

  std::unique_ptr<ImageProcessor> ReleaseImage() { return std::move(image_); }

  void Process(cv::Mat &frame) override {
    if (image_) {
      image_->Process(frame);
    }
  }
};

class GrayscaleDecorator : public FilterDecorator {
public:
  GrayscaleDecorator(std::unique_ptr<ImageProcessor> processor)
      : FilterDecorator(std::move(processor)) {}

  void Process(cv::Mat &frame) override {
    FilterDecorator::Process(frame);
    const auto channel = frame.channels();
    if (channel == 3 || channel == 4) {
      cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
    }
  }
};

class BlurDecorator : public FilterDecorator {
public:
  BlurDecorator(std::unique_ptr<ImageProcessor> processor)
      : FilterDecorator(std::move(processor)) {}

  void Process(cv::Mat &frame) override {
    FilterDecorator::Process(frame);
    cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0);
  }
};

} // namespace decorator
} // namespace image
