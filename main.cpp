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
#include "gui/gui_utils.hpp"
#include "utils/camera.hpp"
#include "utils/conversions.hpp"
#include "utils/gui.hpp"
#include "utils/json.hpp"

using std::cerr;

struct CameraData {
  int id;
  cv::VideoCapture capture;
  cv::Mat frame;
  GLuint texture_id;
  bool is_available{false};
};

void fitInPanel(const ImVec2 &panel_size, float &width, float &height,
                const float &aspect_ratio) {
  if (width > panel_size.x) {
    width = panel_size.x;
    height = width / aspect_ratio;
  }
}

void centerImage(const ImVec2 &panel_size, float &width, float &height) {
  const auto offset_x = (panel_size.x - width) / 2;
  const auto offset_y = (panel_size.y - height) / 2;
  ImVec2 panel_center = ImVec2(offset_x, offset_y);
  ImGui::SetCursorPos(panel_center);
}

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

  std::vector<CameraData> cameras{};

  for (const auto &id : camera_ids) {
    CameraData camera_data{};
    camera_data.id = id;
    camera_data.capture.open(id);

    if (camera_data.capture.isOpened()) {
      camera_data.is_available = true;
    } else {
      camera_data.capture.release();
      camera_data.is_available = false;
    }
    cameras.emplace_back(camera_data);
  }

  cv::VideoCapture cam(current_id);

  // Main Phase

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

    for (auto &camera : cameras) {
      if (camera.is_available) {
        camera.capture.set(cv::CAP_PROP_FPS, 30);
        camera.capture.set(cv::CAP_PROP_FRAME_WIDTH, 720);
        camera.capture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

        camera.capture >> camera.frame;

        camera.texture_id = utils::cvMatToTexture(camera.frame);
      }
    }

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
        // ImGui::Text("Frame Size: %dx%d", frame.cols, frame.rows);

        ImGui::EndChild();
      }

      ImGui::SameLine();

      const auto right_panel_pos = ImVec2(0, 0);
      {
        ImGui::BeginChild("Right Panel", right_panel_pos, true);

        int available_cameras = 0;
        for (const auto &camera : cameras) {
          if (camera.is_available) {
            available_cameras++;
          }
        }

        ImVec2 panel_size = ImGui::GetContentRegionAvail();

        if (available_cameras == 0) {
          ImGui::SetCursorPos(
              ImVec2(panel_size.x / 2 - 100, panel_size.y / 2 - 10));
          ImGui::Text("No camera available");
        } else if (available_cameras == 1) {
          float aspect_ratio = static_cast<float>(cameras.at(0).frame.cols) /
                               static_cast<float>(cameras.at(0).frame.rows);
          float width = panel_size.y * aspect_ratio;
          float height = panel_size.y;

          fitInPanel(panel_size, width, height, aspect_ratio);
          centerImage(panel_size, width, height);

          ImGui::Image(static_cast<ImTextureID>(
                           static_cast<intptr_t>(cameras.at(0).texture_id)),
                       ImVec2(width, height));
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
