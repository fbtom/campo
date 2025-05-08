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
#include "utils/frame.hpp"
#include "utils/gui.hpp"
#include "utils/json.hpp"

using std::cerr;

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

    std::vector<gui::CameraStream> current_camera_streams{
        utils::processCameraFrames(cameras)};

    grid_display.setCameraData(current_camera_streams);

    renderGui(window, app_context, grid_display);

    glfwSwapBuffers(window);
  }

  utils::saveCameraID(current_id);
  utils::shutdownAndCleanUp(window);

  return 0;
}
