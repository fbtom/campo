///
/// @file gui.hpp
/// @author fbtom
/// @brief
/// @date 2025-04-15
///
/// @copyright Copyright (c) 2025
///

#pragma once
// System headers
#include <optional>

// External headers
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include <GLFW/glfw3.h>

// Project headers
#include "gui/gui_utils.hpp"

namespace utils {

void setupIoFlags() {
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
}

std::optional<GLFWwindow *> initWindow() {
  GLFWwindow *window =
      glfwCreateWindow(kWindowWidth, kWindowHeight, "Campo", NULL, NULL);
  if (window == nullptr) {
    return std::nullopt;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  setupIoFlags();

  ImGui::StyleColorsLight();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");

  return window;
}

void initWindowHint() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void shutdownAndCleanUp(GLFWwindow *window) {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
}

} // namespace utils
