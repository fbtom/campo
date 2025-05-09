///
/// @file image_processor.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "opencv2/opencv.hpp"

namespace image {

class ImageProcessor {
public:
  virtual ~ImageProcessor() = default;
  /// @brief Processes the given image frame.
  /// @param frame image to Process.
  virtual void Process(cv::Mat &frame) = 0;
};

class BaseImageProcessor : public ImageProcessor {
public:
  void Process(cv::Mat &frame) override {
    // No processing by default
  }
};

} // namespace image
