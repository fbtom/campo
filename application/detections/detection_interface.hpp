#pragma once
#include "detection_algorithms.hpp"
#include <memory>
#include <opencv2/core.hpp>
#include <vector>

namespace campo {
namespace detections {

class IDetectionAlgorithm {
public:
  virtual ~IDetectionAlgorithm() = default;
  virtual void detect(const cv::Mat &image,
                      std::vector<cv::Rect> &detectedObjects) = 0;
};

} // namespace detections
} // namespace campo
