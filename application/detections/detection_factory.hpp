#pragma once
#include "detection_algorithms.hpp"
#include "detection_interface.hpp"
#include "geometric_shapes_detector.hpp"
#include <memory>
#include <opencv2/core.hpp>
#include <vector>

namespace campo {
namespace detections {

inline std::unique_ptr<IDetectionAlgorithm>
createDetector(DetectionAlgorithm algorithm) {
  switch (algorithm) {
  case DetectionAlgorithm::GEOMETRIC_SHAPES:
    return std::make_unique<GeometricShapesDetector>();
  default:
    return std::make_unique<GeometricShapesDetector>();
  }
}

} // namespace detections
} // namespace campo
