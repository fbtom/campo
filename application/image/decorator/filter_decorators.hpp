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
  virtual void Decorate(cv::Mat &frame) = 0;

protected:
  std::unique_ptr<ImageProcessor> image_processor_;

public:
  FilterDecorator(std::unique_ptr<ImageProcessor> processor)
      : image_processor_(std::move(processor)) {}

  void Process(cv::Mat &frame) override {
    if (image_processor_) {
      image_processor_->Process(frame);
    }
    Decorate(frame);
  }
};

class GrayscaleDecorator : public FilterDecorator {
public:
  GrayscaleDecorator(std::unique_ptr<ImageProcessor> processor)
      : FilterDecorator(std::move(processor)) {}

  void Decorate(cv::Mat &frame) override {
    const auto channel = frame.channels();
    if (channel == 3 || channel == 4) {
      cv::Mat grayFrame{};
      cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
      cv::cvtColor(grayFrame, frame, cv::COLOR_GRAY2BGR);
    }
  }
};

class BlurDecorator : public FilterDecorator {
public:
  BlurDecorator(std::unique_ptr<ImageProcessor> processor)
      : FilterDecorator(std::move(processor)) {}

  void Decorate(cv::Mat &frame) override {
    cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0);
  }
};

} // namespace decorator
} // namespace image
