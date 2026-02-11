#pragma once

#include "utils/frame.hpp"
#include <GLFW/glfw3.h>

auto getFrameSizeProperties(GLFWwindow *window) -> utils::Frame {
  utils::Frame frame{};
  glfwGetFramebufferSize(window, &frame.width, &frame.height);

  return frame;
}
