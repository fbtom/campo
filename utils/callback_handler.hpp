///
/// @file main.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "utils/camera.hpp"
#include <GLFW/glfw3.h>

namespace utils {

/// @brief Callback function for handling keyboard input in the main window
/// based on GLFW callback interface. Shall be used with glfwSetKeyCallback.
void mainWindowCallback(GLFWwindow *win, int key, int scancode, int action,
                        int mods);

} // namespace utils
