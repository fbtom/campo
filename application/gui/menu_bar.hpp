///
/// @file menu_bar.hpp
/// @author fbtom
/// @brief 
/// @date 2025-07-14
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "camera_menu.hpp"
#include "grid_display.hpp"
#include "gui_utils.hpp"
#include "shortcuts.hpp"
#include "utils/callback_handler.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>

namespace gui {
namespace menu_bar {

void renderCampoMenu(GLFWwindow *window) {
  if (ImGui::MenuItem("Exit", kAltF4)) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void renderMenuBar(GLFWwindow *window, utils::AppContext &app_context,
                   GridDisplay &grid_display) {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu(kApplicationName)) {
      renderCampoMenu(window);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Camera")) {
      camera_menu::renderCameraMenu(app_context, grid_display);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}

} // namespace menu_bar
} // namespace gui
