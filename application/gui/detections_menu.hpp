#pragma once
#include "../../utils/camera.hpp"
#include "../detections/detection_factory.hpp"
#include "detection_algorithms.hpp"
#include <imgui.h>
#include <memory>

namespace campo {
namespace gui {

inline void enableDetection(utils::AppContext &app_context) {
  bool wasEnabled = app_context.detectionEnabled;
  if (ImGui::Checkbox("Enable Detection", &app_context.detectionEnabled)) {
    if (app_context.detectionEnabled && !wasEnabled) {
      app_context.detectionSettingsLocked = true;
    } else if (!app_context.detectionEnabled && wasEnabled) {
      app_context.detectionSettingsLocked = false;
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
    if (ImGui::Button("Run Detection",
                      ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
      auto detector = campo::detections::createDetector(
          static_cast<campo::detections::DetectionAlgorithm>(
              app_context.selectedDetectionAlgorithm));

      // TODO: Get current image from camera/processor and run detection
      ImGui::OpenPopup("Detection Results");
    }
  }
}

} // namespace gui
} // namespace campo
