#pragma once

#include <functional>
#include <opencv2/opencv.hpp>
#include <vector>

namespace campo {
namespace detections {

using VecRect = std::vector<cv::Rect>;
using DetectFn = std::function<std::vector<cv::Rect>(const cv::Mat &image)>;

inline VecRect detectGeometricShapes(const cv::Mat &image) {
  VecRect detected_objects;
  cv::Mat gray, blurred, edged;

  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
  cv::Canny(blurred, edged, 50, 150);

  std::vector<std::vector<cv::Point>> contours{};
  cv::findContours(edged, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

  for (const auto &contour : contours) {
    double contour_perimeter = cv::arcLength(contour, true);
    std::vector<cv::Point> approx;
    cv::approxPolyDP(contour, approx, 0.04 * contour_perimeter, true);
    if (cv::contourArea(contour) < 100) {
      continue;
    }

    detected_objects.push_back(cv::boundingRect(approx));
  }
  return detected_objects;
}

enum class DetectionAlgorithm {
  GEOMETRIC_SHAPES,
};

inline DetectFn getDetectionAlgorithm(DetectionAlgorithm algorithm) {
  static std::map<DetectionAlgorithm, DetectFn> algorithm_map = {
      {DetectionAlgorithm::GEOMETRIC_SHAPES, detectGeometricShapes},
  };

  return algorithm_map[algorithm];
}

} // namespace detections
} // namespace campo
