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

/// @brief Setup Keyboard and Gamepad
void setupIoFlags() {
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
}

/// @brief Initializes a GLFW window and sets up ImGui context for rendering a
/// graphical user interface.
/// @return std::optional<GLFWwindow *> A pointer to the created GLFW window if
/// successful, otherwise an empty optional.
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

/// @brief Performs check for correct GLFWwindow intialization.
/// @return GLFWwindow* pointer if successful, otherwise exits the program.
GLFWwindow *initWindowValue() {
  auto window_opt = initWindow();
  if (!window_opt.has_value()) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    exit(-1);
  }
  return window_opt.value();
}

/// @brief Initializes GLFW window hints for OpenGL context version and profile.
/// @note Sets the OpenGL context version to 4.1 and specifies the core
/// profile.
void initWindowHint() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

/// @brief Shuts down ImGui and GLFW, cleans up resources, and terminates the
/// program.
/// @param window Pointer to the GLFW window to be destroyed.
void shutdownAndCleanUp(GLFWwindow *window) {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
}

} // namespace utils
