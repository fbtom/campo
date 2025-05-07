///
/// @file main.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "grid_display.hpp"
#include "utils/callback_handler.hpp"
#include "utils/camera.hpp"
#include "utils/conversions.hpp"
#include "utils/frame.hpp"
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

void renderLeftPanel(GLFWwindow *window, utils::AppContext &app_context,
                     gui::GridDisplay &grid_display) {
  ImGui::BeginChild("Left Panel",
                    ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, 0), true,
                    ImGuiWindowFlags_MenuBar);

  gui::renderMenuBar(window, app_context, grid_display);

  ImGui::Separator();

  if (!app_context.cameras_ptr->empty()) {
    gui::renderCameraDetails(*app_context.cameras_ptr);
  } else {
    ImGui::Text("No cameras available.");
  }

  ImGui::EndChild();
}

void renderRightPanel(gui::GridDisplay &grid_display, int &current_id) {
  ImGui::BeginChild("Right Panel", ImVec2(0, 0), true);

  std::optional<int> choosen_camera = grid_display.renderGrid();
  if (choosen_camera.has_value()) {
    current_id = choosen_camera.value();
  }

  ImGui::EndChild();
}

void initNewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void renderGui(GLFWwindow *window, utils::AppContext &app_context,
               gui::GridDisplay &grid_display) {
  initNewFrame();

  utils::Frame frame{};
  glfwGetFramebufferSize(window, &frame.width, &frame.height);

  const auto main_window_size{ImVec2(frame.width, frame.height)};
  const auto main_window_pos{ImVec2(kWindowPosX, 0)};

  ImGui::SetNextWindowSize(main_window_size);
  ImGui::SetNextWindowPos(main_window_pos);

  if (ImGui::Begin("Campo", NULL,
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                       ImGuiWindowFlags_NoMove)) {
    renderLeftPanel(window, app_context, grid_display);

    ImGui::SameLine();

    renderRightPanel(grid_display, *app_context.current_id_ptr);
    ImGui::End();
  }

  ImGui::Render();
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace gui
