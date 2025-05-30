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

  virtual std::string GetFilterName() const = 0;
};

class GrayscaleDecorator : public FilterDecorator {
  void Decorate(cv::Mat &frame) override {
    const auto channel = frame.channels();
    if (channel == 3 || channel == 4) {
      cv::Mat grayFrame{};
      cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
      cv::cvtColor(grayFrame, frame, cv::COLOR_GRAY2BGR);
    }
  }

public:
  GrayscaleDecorator(std::unique_ptr<ImageProcessor> processor)
      : FilterDecorator(std::move(processor)) {}

  std::string GetFilterName() const override { return "Grayscale"; }
};

class BlurDecorator : public FilterDecorator {
private:
  int blur_intensity_;

  void Decorate(cv::Mat &frame) override {

    int kernel_size = std::max(1, blur_intensity_);
    if (kernel_size % 2 == 0) {
      kernel_size += 1;
    }
    cv::GaussianBlur(frame, frame, cv::Size(kernel_size, kernel_size), 0);
  }

public:
  BlurDecorator(std::unique_ptr<ImageProcessor> processor,
                int blur_intensity = 1)
      : FilterDecorator(std::move(processor)), blur_intensity_(blur_intensity) {
  }

  std::string GetFilterName() const override {
    return "Blur intensity: " + std::to_string(blur_intensity_) + ")";
  }

  void SetBlurIntensity(int intensity) { blur_intensity_ = intensity; }

  int GetBlurIntensity() const { return blur_intensity_; }
};

} // namespace decorator
} // namespace image
