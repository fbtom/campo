
#include "utils/frame.hpp"
#include <GLFW/glfw3.h>

utils::Frame getFrameBuffer(GLFWwindow *window) {
  utils::Frame frame{};
  glfwGetFramebufferSize(window, &frame.width, &frame.height);

  return frame;
}
