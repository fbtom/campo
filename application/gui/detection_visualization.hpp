#pragma once
#include "../../utils/camera.hpp"
#include <array>
#include <map>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

namespace campo {
namespace gui {
enum class fontColor { GREEN, BLUE, RED, CYAN, MAGENTA, YELLOW };

std::map<fontColor, cv::Scalar> kFontColorMap{
    {{fontColor::GREEN, cv::Scalar(0, 255, 0)},
     {fontColor::BLUE, cv::Scalar(255, 0, 0)},
     {fontColor::RED, cv::Scalar(0, 0, 255)},
     {fontColor::CYAN, cv::Scalar(255, 255, 0)},
     {fontColor::MAGENTA, cv::Scalar(255, 0, 255)},
     {fontColor::YELLOW, cv::Scalar(0, 255, 255)}}};

inline void drawDetectionResults(cv::Mat &frame,
                                 const utils::AppContext &app_context) {
  if (!app_context.detectionEnabled || app_context.detectionResults.empty()) {
    return;
  }
  const int fontFace = cv::FONT_HERSHEY_SIMPLEX;
  const double fontScale = 0.6;
  const int thickness = 2;

  constexpr std::array<fontColor, 6> colors = {
      fontColor::GREEN, fontColor::BLUE,    fontColor::RED,
      fontColor::CYAN,  fontColor::MAGENTA, fontColor::YELLOW};

  for (size_t i = 0; i < app_context.detectionResults.size(); ++i) {
    const auto &rect = app_context.detectionResults[i];

    cv::Scalar color = kFontColorMap.at(colors[i % colors.size()]);

    cv::rectangle(frame, rect, color, 2);

    const std::string label = "Object " + std::to_string(i + 1);

    int baseline = 0;
    cv::Size textSize =
        cv::getTextSize(label, fontFace, fontScale, thickness, &baseline);

    cv::Point textOrg(rect.x, rect.y - 5);
    if (textOrg.y < textSize.height) {
      textOrg.y = rect.y + rect.height + textSize.height + 5;
    }

    cv::rectangle(frame, cv::Point(textOrg.x, textOrg.y - textSize.height - 5),
                  cv::Point(textOrg.x + textSize.width, textOrg.y + 5),
                  cv::Scalar(0, 0, 0), cv::FILLED);

    cv::putText(frame, label, textOrg, fontFace, fontScale, color, thickness);
  }

  if (app_context.detectionEnabled) {
    std::string info =
        "No. objects: " + std::to_string(app_context.detectedObjectsCount);
    cv::putText(frame, info, cv::Point(10, 25), fontFace, fontScale,
                kFontColorMap.at(fontColor::GREEN), thickness);
  }
}

} // namespace gui
} // namespace campo
