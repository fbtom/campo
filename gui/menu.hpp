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
#include "shortcuts.hpp"
#include "utils/callback_handler.hpp"
#include "utils/camera.hpp"
#include "utils/conversions.hpp"
#include "utils/frame.hpp"
#include "utils/json.hpp"
#include <imgui.h>

#include <GLFW/glfw3.h>
#include <imgui.h>

namespace gui {

namespace {

void renderCampoMenu(GLFWwindow *window) {
  if (ImGui::MenuItem("Exit", kAltF4)) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void renderCameraMenu(utils::AppContext &app_context,
                      gui::GridDisplay &grid_display) {
  if (ImGui::MenuItem("Update list", kCtrlR)) {
    auto camera_ids = utils::getCameraIDs();
    if (!camera_ids.empty()) {
      utils::refreshCameraList(*app_context.cameras_ptr, camera_ids);
      *app_context.current_id_ptr =
          utils::getValidCameraID(camera_ids, utils::loadCameraID());
    } else {
      app_context.cameras_ptr->clear();
      grid_display.setCameraData({});
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

} // namespace
/// @brief Renders the menu bar with options for exiting the application and
/// updating the camera list.
/// @param window Pointer to the GLFW window.
/// @param app_context Application context containing camera and state
/// information.
/// @param grid_display Grid display for rendering camera data.
void renderMenuBar(GLFWwindow *window, utils::AppContext &app_context,
                   gui::GridDisplay &grid_display) {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Campo")) {
      renderCampoMenu(window);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Camera")) {
      renderCameraMenu(app_context, grid_display);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}

/// @brief Renders detailed information about available cameras.
/// @param cameras A vector of CameraData objects containing camera details.
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

/// @brief Renders the left panel of the GUI, including the menu bar and camera
/// details.
/// @param window Pointer to the GLFW window.
/// @param app_context Application context containing camera and state
/// information.
/// @param grid_display Grid display for rendering camera data.
void renderLeftPanel(GLFWwindow *window, utils::AppContext &app_context,
                     gui::GridDisplay &grid_display) {
  ImGui::BeginChild("Left Panel",
                    ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, 0), true,
                    ImGuiWindowFlags_MenuBar);

  renderMenuBar(window, app_context, grid_display);

  ImGui::Separator();

  if (!app_context.cameras_ptr->empty()) {
    renderCameraDetails(*app_context.cameras_ptr);
  } else {
    ImGui::Text("No cameras available.");
  }

  ImGui::EndChild();
}

/// @brief Renders the right panel of the GUI, including the camera grid
/// display.
/// @param grid_display Grid display for rendering camera data.
/// @param current_id Reference to the current camera ID.
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

/// @brief Initializes a new frame for rendering the GUI.
void initNewFrame();

/// @brief Renders the entire GUI, including both left and right panels.
/// @param window Pointer to the GLFW window.
/// @param app_context Application context containing camera and state
/// information.
/// @param grid_display Grid display for rendering camera data.
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
