///
/// @file filter_decorators.hpp
/// @author fbtom
/// @brief Defines decorator classes for applying image filters.
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "image_processor.hpp"
#include "opencv2/opencv.hpp"
#include <memory>

namespace utils {

class FilterDecorator : public ImageProcessor {
protected:
  std::unique_ptr<ImageProcessor> wrapped_processor_;

public:
  FilterDecorator(std::unique_ptr<ImageProcessor> processor)
      : wrapped_processor_(std::move(processor)) {}

  void Process(cv::Mat &frame) override {
    if (wrapped_processor_) {
      wrapped_processor_->Process(frame);
    }
  }
};

class GrayscaleDecorator : public FilterDecorator {
public:
  GrayscaleDecorator(std::unique_ptr<ImageProcessor> processor)
      : FilterDecorator(std::move(processor)) {}

  void Process(cv::Mat &frame) override {
    FilterDecorator::Process(frame);
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
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

} // namespace utils
