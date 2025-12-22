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
#include "application/gui/grid_display.hpp"
#include "application/gui/gui_utils.hpp"
#include "application/gui/menu.hpp"
#include "application/image/image_process/image_processor_manager.hpp"
#include "application/image/region/region_selector.hpp"
#include "common/camera_stream.hpp"
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

  gui::GridDisplay grid_display{};
  auto cam_ids = utils::getCameraIDs();
  auto current_id{utils::getValidCameraID(cam_ids, utils::loadCameraID())};
  auto app_context{utils::initializeAppContext(window, current_id)};

  utils::updateCameraList(*app_context.cameras_ptr, cam_ids);

  glfwSetKeyCallback(window, utils::mainWindowCallback);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    auto selected_cam = grid_display.IsGridView()
                            ? std::nullopt
                            : grid_display.GetSelectedCameraId();
    auto cam_streams{utils::processCameraFrames(&app_context, selected_cam)};
    grid_display.SetCameraData(cam_streams);

    renderGui(window, app_context, grid_display);

    glfwSwapBuffers(window);
  }

  utils::saveCameraID(current_id);
  utils::shutdownAndCleanUp(window);

  return 0;
}
