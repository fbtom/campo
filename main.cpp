///
/// @file main.cpp
/// @author fbtom
/// @brief
/// @date 2025-03-07
///
/// @copyright Copyright (c) 2025
///

#define GL_SILENCE_DEPRECATION

// System headers
#include <iostream>
#include <vector>
// External headers
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "opencv2/opencv.hpp"
#include <GLFW/glfw3.h>
// Project headers
#include "gui/grid_display.hpp"
#include "gui/gui_utils.hpp"
#include "gui/menu.hpp"
#include "utils/callback_handler.hpp"
#include "utils/camera.hpp"
#include "utils/conversions.hpp"
#include "utils/gui.hpp"
#include "utils/json.hpp"

using std::cerr;
struct Frame {
  int width;
  int height;
};

void processCameraFrames(
    std::vector<utils::CameraData> &cameras,
    std::vector<gui::CameraStream> &current_camera_streams) {
  current_camera_streams.clear();
  for (auto &camera : cameras) {
    if (camera.is_available) {
      camera.capture.set(cv::CAP_PROP_FPS, 60);

      camera.capture >> camera.frame;

      if (!camera.frame.empty()) {
        camera.texture_id = utils::cvMatToTexture(camera.frame);
        current_camera_streams.push_back(
            {static_cast<ImTextureID>(camera.texture_id), camera.frame.cols,
             camera.frame.rows, camera.id});
      }
    }
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

  Frame frame{};
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

int main() {
  if (!glfwInit()) {
    return -1;
  }
  utils::initWindowHint();

  auto window = utils::initWindowValue();

  std::vector<utils::CameraData> cameras{};
  gui::GridDisplay grid_display{};
  int current_id{0};

  utils::AppContext app_context{};
  app_context.cameras_ptr = &cameras;
  app_context.current_id_ptr = &current_id;
  glfwSetWindowUserPointer(window, &app_context);

  auto initial_camera_ids = utils::getCameraIDs();
  utils::refreshCameraList(cameras, initial_camera_ids);
  current_id =
      utils::getValidCameraID(initial_camera_ids, utils::loadCameraID());

  glfwSetKeyCallback(window, utils::mainWindowCallback);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    std::vector<gui::CameraStream> current_camera_streams{};
    processCameraFrames(cameras, current_camera_streams);

    grid_display.setCameraData(current_camera_streams);

    renderGui(window, app_context, grid_display);

    glfwSwapBuffers(window);
  }

  utils::saveCameraID(current_id);
  utils::shutdownAndCleanUp(window);

  return 0;
}
