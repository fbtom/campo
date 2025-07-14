///
/// @file menu.hpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "grid_display.hpp"
#include "panel_renderer.hpp"
#include "utils/frame.hpp"
#include "gui_utils.hpp"
#include <imgui.h>
#include <GLFW/glfw3.h>

// Forward declarations
extern void ImGui_ImplOpenGL3_NewFrame();
extern void ImGui_ImplGlfw_NewFrame();
extern void ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

namespace gui {

/// @brief Renders the menu bar with options for exiting the application and
/// updating the camera list.
/// @param window Pointer to the GLFW window.
/// @param app_context Application context containing camera and state
/// information.
/// @param grid_display Grid display for rendering camera data.
void renderMenuBar(GLFWwindow *window, utils::AppContext &app_context,
                   GridDisplay &grid_display) {
  menu_bar::renderMenuBar(window, app_context, grid_display);
}

/// @brief Renders detailed information about available cameras.
/// @param cameras A vector of CameraData objects containing camera details.
void renderCameraDetails(const std::vector<utils::CameraData> &cameras) {
  camera_menu::renderCameraDetails(cameras);
}

/// @brief Renders the left panel of the GUI, including the menu bar and camera
/// details.
/// @param window Pointer to the GLFW window.
/// @param app_context Application context containing camera and state
/// information.
/// @param grid_display Grid display for rendering camera data.
void renderLeftPanel(GLFWwindow *window, utils::AppContext &app_context,
                     GridDisplay &grid_display) {
  panel_renderer::renderLeftPanel(window, app_context, grid_display);
}

/// @brief Renders the right panel of the GUI, including the camera grid
/// display.
/// @param grid_display Grid display for rendering camera data.
/// @param current_id Reference to the current camera ID.
void renderRightPanel(GridDisplay &grid_display, int &current_id,
                      utils::AppContext &app_context) {
  panel_renderer::renderRightPanel(grid_display, current_id, app_context);
}

/// @brief Initializes a new frame for rendering the GUI.
void initNewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

/// @brief Renders the entire GUI, including both left and right panels.
/// @param window Pointer to the GLFW window.
/// @param app_context Application context containing camera and state
/// information.
/// @param grid_display Grid display for rendering camera data.
void renderGui(GLFWwindow *window, utils::AppContext &app_context,
               GridDisplay &grid_display) {
  initNewFrame();

  utils::Frame frame{};
  glfwGetFramebufferSize(window, &frame.width, &frame.height);

  const auto main_window_size{ImVec2(frame.width, frame.height)};
  const auto main_window_pos{ImVec2(kWindowPosX, 0)};

  ImGui::SetNextWindowSize(main_window_size);
  ImGui::SetNextWindowPos(main_window_pos);

  if (ImGui::Begin(kApplicationName, NULL,
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                       ImGuiWindowFlags_NoMove)) {
    renderLeftPanel(window, app_context, grid_display);

    ImGui::SameLine();

    renderRightPanel(grid_display, *app_context.current_id_ptr, app_context);
    ImGui::End();
  }

  ImGui::Render();
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace gui
