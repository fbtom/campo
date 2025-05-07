#include "callback_handler.hpp"
#include "camera.hpp"

namespace utils {

void altF4Callback(GLFWwindow *win, int key, int scancode, int action,
                   int mods) {
  if (key == GLFW_KEY_F4 && action == GLFW_PRESS && (mods & GLFW_MOD_ALT)) {
    glfwSetWindowShouldClose(win, GLFW_TRUE);
  }
}

void crtlRCallback(GLFWwindow *win, int key, int scancode, int action, int mods,
                   AppContext *context) {
  if (key == GLFW_KEY_R && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)) {
    auto new_camera_ids = utils::getCameraIDs();
    if (context->cameras_ptr) {
      refreshCameraList(*context->cameras_ptr, new_camera_ids);
    }
    if (context->current_id_ptr) {
      *context->current_id_ptr =
          utils::getValidCameraID(new_camera_ids, *context->current_id_ptr);
    }
  }
}

void mainWindowCallback(GLFWwindow *win, int key, int scancode, int action,
                        int mods) {
  auto *context = static_cast<AppContext *>(glfwGetWindowUserPointer(win));
  if (!context) {
    return;
  }

  altF4Callback(win, key, scancode, action, mods);
  crtlRCallback(win, key, scancode, action, mods, context);
}

} // namespace utils
