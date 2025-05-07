#pragma once

#include "utils/camera.hpp"
#include <GLFW/glfw3.h>

namespace utils {

void mainWindowCallback(GLFWwindow *win, int key, int scancode, int action,
                        int mods);

} // namespace utils
