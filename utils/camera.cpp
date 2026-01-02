///
/// @file camera.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#include "utils/camera.hpp"
#include "application/gui/detection_visualization.hpp"
#include "application/gui/detections_menu.hpp"
#include "application/image/image_process/image_processor_manager.hpp"
#include <GLFW/glfw3.h>

namespace utils {

auto getCameraIDs() -> std::vector<int> {
  std::vector<int> ids{};
  cv::VideoCapture cap{};
  for (int i = 0; i < 10; ++i) {
    if (cap.open(i)) {
      ids.push_back(i);
      cap.release();
    }
  }

  return ids;
}

auto getValidCameraID(const std::vector<int> &camera_ids,
                      const std::optional<int> saved_id) -> int {
  if (camera_ids.empty()) {
    return -1;
  }

  if (saved_id.has_value()) {
    for (int id : camera_ids) {
      if (id == saved_id.value()) {
        return id;
      }
    }
  }

  return camera_ids[0];
}

void updateCameraList(std::vector<CameraData> &container,
                      const std::vector<int> &new_camera_ids) {
  container.clear();
  for (const auto &id : new_camera_ids) {
    CameraData camera_data{};
    camera_data.id = id;
    camera_data.capture.open(id);

    if (camera_data.capture.isOpened()) {
      camera_data.is_available = true;
      camera_data.processor_manager =
          std::make_unique<image::process::ImageProcessorManager>();
      camera_data.command_history =
          std::make_unique<image::history::CommandHistory>();
    } else {
      camera_data.capture.release();
      camera_data.is_available = false;
    }
    container.emplace_back(std::move(camera_data));
  }
}

void updateCameraTexture(CameraData &camera) {
  bool need_new_texture = (camera.texture_id == 0) ||
                          (camera.frame.cols != camera.last_frame_width) ||
                          (camera.frame.rows != camera.last_frame_height);

  if (need_new_texture) {
    if (camera.texture_id != 0) {
      glDeleteTextures(1, &camera.texture_id);
    }
    camera.texture_id = cvMatToTexture(camera.frame);
    camera.last_frame_width = camera.frame.cols;
    camera.last_frame_height = camera.frame.rows;
  } else {
    updateTextureData(camera.texture_id, camera.frame);
  }
}

std::vector<common::CameraStream>
processCameraFrames(utils::AppContext *app_context,
                    std::optional<int> selected_camera_id) {
  auto camera_streams = std::vector<common::CameraStream>{};
  auto &cameras = *app_context->cameras_ptr;

  // When in single camera view, only process the selected camera
  if (selected_camera_id.has_value()) {
    for (auto &camera : cameras) {
      auto &frame = camera.frame;
      if (camera.is_available && camera.id == selected_camera_id.value()) {
        camera.capture.set(cv::CAP_PROP_FPS, 60);
        camera.capture >> frame;

        if (!frame.empty()) {
          if (camera.processor_manager) {
            camera.processor_manager->processFrame(frame);
          }

          if (app_context && app_context->detection_enabled) {
            campo::gui::runDetectionOnFrame(*app_context);
            // TODO implement detection tracking across frames
            // requires comparison between previous and current detected ids
            // based on centroids or bounding boxes positions
            campo::gui::drawDetectionResults(frame, *app_context);
          }

          updateCameraTexture(camera);
          camera_streams.push_back({static_cast<ImTextureID>(camera.texture_id),
                                    frame.cols, frame.rows, camera.id});
        }
        break;
      } else {
        // For non-selected cameras, add the last frame but no update
        if (camera.is_available && !camera.frame.empty()) {
          camera_streams.push_back({static_cast<ImTextureID>(camera.texture_id),
                                    camera.frame.cols, camera.frame.rows,
                                    camera.id});
        }
      }
    }
  } else {
    // Process cameras in grid view
    for (auto &camera : cameras) {
      if (camera.is_available) {
        camera.capture.set(cv::CAP_PROP_FPS, 60);
        camera.capture >> camera.frame;

        if (!camera.frame.empty()) {
          if (camera.processor_manager) {
            camera.processor_manager->processFrame(camera.frame);
          }
          updateCameraTexture(camera);
          camera_streams.push_back({static_cast<ImTextureID>(camera.texture_id),
                                    camera.frame.cols, camera.frame.rows,
                                    camera.id});
        }
      }
    }
  }

  return camera_streams;
}

auto initializeAppContext(GLFWwindow *window, int current_id)
    -> utils::AppContext {
  utils::AppContext app_context{
      std::make_unique<std::vector<utils::CameraData>>(),
      std::make_unique<int>(current_id),
      std::make_unique<image::history::CommandHistory>(),
      std::make_unique<image::process::ImageProcessorManager>(),
      std::make_unique<image::region::RegionSelector>()};

  glfwSetWindowUserPointer(window, &app_context);

  return app_context;
}

} // namespace utils
