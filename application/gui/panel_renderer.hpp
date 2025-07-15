///
/// @file panel_renderer.hpp
/// @author fbtom
/// @brief 
/// @date 2025-07-14
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "camera_menu.hpp"
#include "filter_menu.hpp"
#include "grid_display.hpp"
#include "menu_bar.hpp"
#include "utils/callback_handler.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <optional>

namespace gui {
namespace panel_renderer {

void renderLeftPanel(GLFWwindow *window, utils::AppContext &app_context,
                     GridDisplay &grid_display) {
  ImGui::BeginChild("Left Panel",
                    ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, 0), true,
                    ImGuiWindowFlags_MenuBar);

  menu_bar::renderMenuBar(window, app_context, grid_display);

  ImGui::Separator();

  if (!app_context.cameras_ptr->empty()) {
    camera_menu::renderCameraDetails(*app_context.cameras_ptr);
  } else {
    ImGui::Text("No cameras available.");
  }

  filter_menu::renderEffectsMenu(app_context, grid_display.IsGridView());

  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 32);
  ImGui::Separator();
  grid_display.RenderReturnButton();

  ImGui::EndChild();
}

void renderRightPanel(GridDisplay &grid_display, int &current_id,
                      utils::AppContext &app_context) {
  ImGui::BeginChild("Right Panel", ImVec2(0, 0), true);

  std::optional<int> chosen_camera =
      grid_display.RenderGrid(app_context.region_selector_ptr.get());
  if (chosen_camera.has_value()) {
    current_id = chosen_camera.value();
  }

  ImGui::EndChild();
}

} // namespace panel_renderer
} // namespace gui
