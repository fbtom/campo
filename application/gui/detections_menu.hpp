#pragma once
#include "../../utils/camera.hpp"
#include "../detections/detection_algorithms.hpp"
#include <imgui.h>
#include <opencv2/opencv.hpp>

namespace campo {
namespace gui {

inline void enableDetection(utils::AppContext &app_context) {
  bool wasEnabled = app_context.detection_enabled;
  if (ImGui::Checkbox("Enable Detection", &app_context.detection_enabled)) {
    if (app_context.detection_enabled && !wasEnabled) {
      app_context.detection_settings_locked = true;
    } else if (!app_context.detection_enabled && wasEnabled) {
      app_context.detection_settings_locked = false;
      app_context.detection_results.clear();
      app_context.detected_objects_count = 0;
    }
  }
}

inline void runDetectionOnFrame(utils::AppContext &app_context) {
  if (!app_context.detection_enabled) {
    return;
  }

  if (app_context.cameras_ptr && app_context.current_id_ptr) {
    int current_camera_id = *app_context.current_id_ptr;

    for (auto &camera : *app_context.cameras_ptr) {
      if (camera.id == current_camera_id && camera.is_available &&
          !camera.frame.empty()) {
        auto detector = campo::detections::getDetectionAlgorithm(
            app_context.selected_detection_algorithm);

        app_context.detection_results = detector(camera.frame);
        app_context.detected_objects_count =
            app_context.detection_results.size();
        break;
      }
    }
  }
}

inline void showDetectionsMenu(utils::AppContext &app_context) {
  ImGui::Separator();
  ImGui::Text("Object Detection");

  enableDetection(app_context);

  if (!app_context.detection_settings_locked) {
    int current = static_cast<int>(app_context.selected_detection_algorithm);
    const char *items[] = {"Geometric shapes"};
    if (ImGui::Combo("Algorithm", &current, items, IM_ARRAYSIZE(items))) {
      app_context.selected_detection_algorithm =
          static_cast<campo::detections::DetectionAlgorithm>(current);
    }
  } else {
    const char *items[] = {"Geometric shapes"};
    ImGui::BeginDisabled();
    int current = static_cast<int>(app_context.selected_detection_algorithm);
    ImGui::Combo("Algorithm", &current, items, IM_ARRAYSIZE(items));
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
                       "(Disable detection to change)");
  }

  if (app_context.detection_enabled) {
    ImGui::Text("Detection Status: Active");

  } else {
    ImGui::Text("Detection Status: Disabled");
  }
}

} // namespace gui
} // namespace campo
