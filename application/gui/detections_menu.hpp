#pragma once
#include "../../utils/camera.hpp"
#include "../detections/detection_factory.hpp"
#include "detection_algorithms.hpp"
#include <imgui.h>
#include <memory>
#include <opencv2/opencv.hpp>

namespace campo {
namespace gui {

inline void enableDetection(utils::AppContext &app_context) {
  bool wasEnabled = app_context.detectionEnabled;
  if (ImGui::Checkbox("Enable Detection", &app_context.detectionEnabled)) {
    if (app_context.detectionEnabled && !wasEnabled) {
      app_context.detectionSettingsLocked = true;
    } else if (!app_context.detectionEnabled && wasEnabled) {
      app_context.detectionSettingsLocked = false;
      app_context.detectionResults.clear();
      app_context.detectedObjectsCount = 0;
    }
  }
}

inline void runDetectionOnFrame(utils::AppContext &app_context) {
  if (!app_context.detectionEnabled) {
    return;
  }

  if (app_context.cameras_ptr && app_context.current_id_ptr) {
    int current_camera_id = *app_context.current_id_ptr;

    for (auto &camera : *app_context.cameras_ptr) {
      if (camera.id == current_camera_id && camera.is_available &&
          !camera.frame.empty()) {
        auto detector = campo::detections::createDetector(
            static_cast<campo::detections::DetectionAlgorithm>(
                app_context.selectedDetectionAlgorithm));

        detector->detect(camera.frame, app_context.detectionResults);
        app_context.detectedObjectsCount = app_context.detectionResults.size();
        break;
      }
    }
  }
}

inline void showDetectionsMenu(utils::AppContext &app_context) {
  ImGui::Separator();
  ImGui::Text("Object Detection");

  enableDetection(app_context);

  if (!app_context.detectionSettingsLocked) {
    int current = app_context.selectedDetectionAlgorithm;
    const char *items[] = {"Geometric shapes"};
    if (ImGui::Combo("Algorithm", &current, items, IM_ARRAYSIZE(items))) {
      app_context.selectedDetectionAlgorithm = current;
    }
  } else {
    const char *items[] = {"Geometric shapes"};
    ImGui::BeginDisabled();
    int current = app_context.selectedDetectionAlgorithm;
    ImGui::Combo("Algorithm", &current, items, IM_ARRAYSIZE(items));
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
                       "(Disable detection to change)");
  }

  if (app_context.detectionEnabled) {
    ImGui::Text("Detection Status: Active");

  } else {
    ImGui::Text("Detection Status: Disabled");
  }
}

} // namespace gui
} // namespace campo
