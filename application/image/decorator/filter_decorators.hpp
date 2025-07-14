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
#include <optional>

namespace image {
namespace decorator {

class FilterDecorator : public ImageProcessor {
  virtual void Decorate(cv::Mat &frame) = 0;
  void DecorateRegion(cv::Mat &frame,
                      std::optional<cv::Rect> region_opt = std::nullopt) {
    if (!region_opt.has_value()) {
      Decorate(frame);
      return;
    }
    auto region = region_opt.value();
    if (region.x >= 0 && region.y >= 0 &&
        region.x + region.width <= frame.cols &&
        region.y + region.height <= frame.rows) {
      cv::Mat region_mat = frame(region);
      Decorate(region_mat);
    }
  }

  std::unique_ptr<ImageProcessor> image_processor_;
  std::optional<cv::Rect> processing_region_;

public:
  FilterDecorator(std::unique_ptr<ImageProcessor> processor)
      : image_processor_(std::move(processor)) {}

  void Process(cv::Mat &frame) override {
    if (image_processor_) {
      image_processor_->Process(frame);
    }

    DecorateRegion(frame, processing_region_);
  }

  void SetProcessingRegion(const std::optional<cv::Rect> &region) {
    processing_region_ = region;
  }

  std::optional<cv::Rect> GetProcessingRegion() const {
    return processing_region_;
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
    return "Blur (intensity: " + std::to_string(blur_intensity_) + ")";
  }

  void SetBlurIntensity(int intensity) { blur_intensity_ = intensity; }

  int GetBlurIntensity() const { return blur_intensity_; }
};

class SepiaDecorator : public FilterDecorator {
  void Decorate(cv::Mat &frame) override {
    for (int y = 0; y < frame.rows; y++) {
      for (int x = 0; x < frame.cols; x++) {
        cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
        float b = pixel[0];
        float g = pixel[1];
        float r = pixel[2];

        pixel[0] =
            cv::saturate_cast<uchar>(0.272 * r + 0.534 * g + 0.131 * b); // B
        pixel[1] =
            cv::saturate_cast<uchar>(0.349 * r + 0.686 * g + 0.168 * b); // G
        pixel[2] =
            cv::saturate_cast<uchar>(0.393 * r + 0.769 * g + 0.189 * b); // R
      }
    }
  }

public:
  SepiaDecorator(std::unique_ptr<ImageProcessor> processor)
      : FilterDecorator(std::move(processor)) {}

  std::string GetFilterName() const override { return "Sepia"; }
};

class EdgeDetectionDecorator : public FilterDecorator {
private:
  int threshold1_;
  int threshold2_;

  void Decorate(cv::Mat &frame) override {
    cv::Mat gray, edges;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, threshold1_, threshold2_);
    cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);
  }

public:
  EdgeDetectionDecorator(std::unique_ptr<ImageProcessor> processor,
                         int threshold1 = 50, int threshold2 = 150)
      : FilterDecorator(std::move(processor)), threshold1_(threshold1),
        threshold2_(threshold2) {}

  std::string GetFilterName() const override {
    return "Edge Detection (th1:" + std::to_string(threshold1_) +
           " th2:" + std::to_string(threshold2_) + ")";
  }

  void SetThresholds(int threshold1, int threshold2) {
    threshold1_ = threshold1;
    threshold2_ = threshold2;
  }

  int GetThreshold1() const { return threshold1_; }
  int GetThreshold2() const { return threshold2_; }
};

} // namespace decorator
} // namespace image
