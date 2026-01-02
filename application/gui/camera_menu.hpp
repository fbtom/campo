///
/// @file camera_menu.hpp
/// @author fbtom
/// @brief
/// @date 2025-07-14
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "grid_display.hpp"
#include "shortcuts.hpp"
#include "utils/camera.hpp"
#include "utils/json.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <opencv2/opencv.hpp>

namespace gui {
namespace camera_menu {

void renderCameraMenu(utils::AppContext &app_context,
                      GridDisplay &grid_display) {
  if (ImGui::MenuItem("Update list", kCtrlR)) {
    auto camera_ids = utils::getCameraIDs();
    if (!camera_ids.empty()) {
      utils::updateCameraList(*app_context.cameras_ptr, camera_ids);
      *app_context.current_id_ptr =
          utils::getValidCameraID(camera_ids, utils::loadCameraID());
    } else {
      app_context.cameras_ptr->clear();
      grid_display.SetCameraData({});
      *app_context.current_id_ptr = -1;
    }
  }
}

void cameraDetails(const utils::CameraData &camera) {
  ImGui::Text("Camera ID: %d", camera.id);
  ImGui::Text("Resolution: %.fx%.f",
              camera.capture.get(cv::CAP_PROP_FRAME_WIDTH),
              camera.capture.get(cv::CAP_PROP_FRAME_HEIGHT));
  ImGui::Text("FPS: %d",
              static_cast<int>(camera.capture.get(cv::CAP_PROP_FPS)));
  ImGui::Text("Exposure: %.2f", camera.capture.get(cv::CAP_PROP_EXPOSURE));
  ImGui::Text("Zoom: %.2f", camera.capture.get(cv::CAP_PROP_ZOOM));
  ImGui::Text("Focus: %.2f", camera.capture.get(cv::CAP_PROP_FOCUS));
}

void renderCameraDetails(const std::vector<utils::CameraData> &cameras) {
  if (!cameras.empty()) {
    for (const auto &camera : cameras) {
      if (camera.is_available) {
        cameraDetails(camera);
        ImGui::Separator();
      }
    }
  } else {
    ImGui::Text("No cameras available.");
  }
}

} // namespace camera_menu
} // namespace gui
