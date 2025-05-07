#pragma once

#include "grid_display.hpp"
#include "utils/callback_handler.hpp"
#include "utils/camera.hpp"
#include "utils/conversions.hpp"
#include "utils/json.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>

namespace gui {

void renderMenuBar(GLFWwindow *window, utils::AppContext &app_context,
                   gui::GridDisplay &grid_display) {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      if (ImGui::MenuItem("Exit", "Alt+F4")) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
      }
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Camera")) {
      if (ImGui::MenuItem("Search for connected cameras")) {
        auto camera_ids = utils::getCameraIDs();
        if (!camera_ids.empty()) {
          utils::refreshCameraList(*app_context.cameras_ptr, camera_ids);
          *app_context.current_id_ptr =
              utils::getValidCameraID(camera_ids, utils::loadCameraID());
        } else {
          std::cerr << "Info: No cameras found during scan." << std::endl;
          app_context.cameras_ptr->clear();
          grid_display.setCameraData({});
          *app_context.current_id_ptr = -1;
        }
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}

void renderCameraDetails(const std::vector<utils::CameraData> &cameras) {
  if (!cameras.empty()) {
    for (const auto &camera : cameras) {
      if (camera.is_available) {
        ImGui::Text("Camera ID: %d", camera.id);
        ImGui::Text("Resolution: %.fx%.f",
                    camera.capture.get(cv::CAP_PROP_FRAME_WIDTH),
                    camera.capture.get(cv::CAP_PROP_FRAME_HEIGHT));
        ImGui::Text("FPS: %d",
                    static_cast<int>(camera.capture.get(cv::CAP_PROP_FPS)));
        ImGui::Text("Exposure: %.2f",
                    camera.capture.get(cv::CAP_PROP_EXPOSURE));
        ImGui::Text("Zoom: %.2f", camera.capture.get(cv::CAP_PROP_ZOOM));
        ImGui::Text("Focus: %.2f", camera.capture.get(cv::CAP_PROP_FOCUS));
        ImGui::Separator();
      }
    }
  } else {
    ImGui::Text("No cameras available.");
  }
}

} // namespace gui
