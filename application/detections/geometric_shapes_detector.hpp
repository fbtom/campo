#pragma once
#include "detection_interface.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

namespace campo {
namespace detections {

class GeometricShapesDetector : public IDetectionAlgorithm {
public:
  void detect(const cv::Mat &image,
              std::vector<cv::Rect> &detectedObjects) override {
    detectedObjects.clear();

    cv::Mat gray, blurred, edged;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
    cv::Canny(blurred, edged, 50, 150);
    std::vector<std::vector<cv::Point>> contours{};

    cv::findContours(edged, contours, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);
    for (const auto &contour : contours) {
      double contour_perimeter = cv::arcLength(contour, true);
      std::vector<cv::Point> approx;
      cv::approxPolyDP(contour, approx, 0.04 * contour_perimeter, true);
      if (cv::contourArea(contour) < 100)
        continue;
      cv::Rect bounding = cv::boundingRect(approx);

      detectedObjects.push_back(bounding);
    }
  }
};

} // namespace detections
} // namespace campo
