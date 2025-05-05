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
// External headers
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "opencv2/opencv.hpp"
#include <GLFW/glfw3.h>
// Project headers
#include "gui/gui_utils.hpp"
#include "utils/camera.hpp"
#include "utils/conversions.hpp"
#include "utils/gui.hpp"
#include "utils/json.hpp"

using std::cerr;

int main() {
  // Init Phase
  if (!glfwInit()) {
    return -1;
  }

  utils::initWindowHint();

  auto window_opt = utils::initWindow();
  if (window_opt.has_value() == false) {
    cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  auto window = window_opt.value();

  auto camera_ids = utils::getCameraIDs();
  auto current_id = utils::getValidCameraID(camera_ids, utils::loadCameraID());

  cv::VideoCapture cam(current_id);

  // Main Phase
  cv::Mat frame{};
  GLuint textureId = 0;

  glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode,
                                int action, int mods) {
    if (key == GLFW_KEY_F4 && action == GLFW_PRESS && (mods & GLFW_MOD_ALT)) {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS &&
        (mods & GLFW_MOD_CONTROL)) {
      auto camera_ids = utils::getCameraIDs();
    }
  });

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    cam >> frame;

    if (frame.empty()) {
      break;
    }
    textureId = utils::cvMatToTexture(frame);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    int width{};
    int height{};
    glfwGetFramebufferSize(window, &width, &height);

    const auto main_window_size{ImVec2(width, height)};
    const auto main_window_pos{ImVec2(kWindowPosX, 0)};

    ImGui::SetNextWindowSize(main_window_size);
    ImGui::SetNextWindowPos(main_window_pos);

    if (ImGui::Begin("Campo", NULL,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                         ImGuiWindowFlags_NoMove)) {
      const auto total_size = ImGui::GetContentRegionAvail();
      const auto total_width = total_size.x;
      const auto total_height = total_size.y;

      const auto left_panel_width = total_width * 0.25f;
      const auto left_panel_pos = ImVec2(left_panel_width, 0);
      {
        ImGui::BeginChild("Left Panel", left_panel_pos, true,
                          ImGuiWindowFlags_MenuBar);

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
            }
            ImGui::EndMenu();
          }
          ImGui::EndMenuBar();
        }

        ImGui::Separator();

        ImGui::Separator();

        ImGui::Text("Camera ID: %d", current_id);
        ImGui::Text("FPS: %.2f", 1000.0f / ImGui::GetIO().DeltaTime);
        ImGui::Text("Frame Size: %dx%d", frame.cols, frame.rows);

        ImGui::EndChild();
      }

      ImGui::SameLine();

      const auto right_panel_pos = ImVec2(0, 0);
      {
        ImGui::BeginChild("Right Panel", right_panel_pos, true);

        if (textureId) {
          ImVec2 panel_size = ImGui::GetContentRegionAvail();
          float aspect_ratio = (float)frame.cols / (float)frame.rows;
          float desiredWidth = panel_size.y * aspect_ratio;

          ImGui::Image((ImTextureID)(intptr_t)textureId,
                       ImVec2(desiredWidth, panel_size.y));
        }

        ImGui::EndChild();
      }

      ImGui::End();
    }

    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  utils::saveCameraID(current_id);

  // Shutdown Phase
  utils::shutdownAndCleanUp(window);

  return 0;
}
