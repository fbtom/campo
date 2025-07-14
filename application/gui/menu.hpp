///
/// @file main.cpp
/// @author fbtom
/// @brief
/// @date 2025-05-08
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/camera_view/camera_view_adapter.hpp"
#include "grid_display.hpp"
#include "shortcuts.hpp"
#include "utils/callback_handler.hpp"
#include "utils/camera.hpp"
#include "utils/conversions.hpp"
#include "utils/frame.hpp"
#include "utils/gui.hpp"
#include "utils/json.hpp"
#include <imgui.h>

#include <GLFW/glfw3.h>
#include <imgui.h>

#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/filter/add_filter_command.hpp"
#include "application/image/filter/filter_command.hpp"
#include "application/image/filter/filter_command_receiver.hpp"
#include "application/image/filter/remove_filter_command.hpp"
#include "application/image/filter/remove_specific_filter_command.hpp"
#include "application/image/filter/toggle_filter_command.hpp"
#include "application/image/filter/add_filter_at_index_command.hpp"
#include "application/image/history/command_history.hpp"
#include "application/image/image_process/image_processor_manager.hpp"

#include <memory>

namespace gui {

namespace {

void renderCampoMenu(GLFWwindow *window) {
  if (ImGui::MenuItem("Exit", kAltF4)) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void renderCameraMenu(utils::AppContext &app_context,
                      GridDisplay &grid_display) {
  if (ImGui::MenuItem("Update list", kCtrlR)) {
    auto camera_ids = utils::getCameraIDs();
    if (!camera_ids.empty()) {
      utils::refreshCameraList(*app_context.cameras_ptr, camera_ids);
      *app_context.current_id_ptr =
          utils::getValidCameraID(camera_ids, utils::loadCameraID());
    } else {
      app_context.cameras_ptr->clear();
      grid_display.SetCameraData({});
      *app_context.current_id_ptr = -1;
    }
  }
}

void cameraDetails(const utils::CameraData &camera) {
  ImGui::Text("Camera ID: %d", camera.id);
  ImGui::Text("Resolution: %.fx%.f",
              camera.capture.get(cv::CAP_PROP_FRAME_WIDTH),
              camera.capture.get(cv::CAP_PROP_FRAME_HEIGHT));
  ImGui::Text("FPS: %d",
              static_cast<int>(camera.capture.get(cv::CAP_PROP_FPS)));
  ImGui::Text("Exposure: %.2f", camera.capture.get(cv::CAP_PROP_EXPOSURE));
  ImGui::Text("Zoom: %.2f", camera.capture.get(cv::CAP_PROP_ZOOM));
  ImGui::Text("Focus: %.2f", camera.capture.get(cv::CAP_PROP_FOCUS));
}

template <typename FilterType>
void renderFilterButton(
    const char *label,
    image::process::ImageProcessorManager &image_processor_manager,
    image::history::CommandHistory &command_history,
    utils::AppContext &app_context) {
  if (ImGui::Button(label, ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
    auto receiver = std::make_shared<image::filter::FilterCommandReceiver>(
        &image_processor_manager);

    auto filter = std::make_unique<FilterType>(
        std::make_unique<image::BaseImageProcessor>());

    std::optional<cv::Rect> region = std::nullopt;
    if (app_context.region_selector_ptr &&
        app_context.region_selector_ptr->GetMode() ==
            image::region::FilterMode::kPartialRegion &&
        app_context.region_selector_ptr->HasValidSelection()) {
      region = app_context.region_selector_ptr->GetRegion();
    }

    if (region.has_value()) {
      filter->SetProcessingRegion(region);
    }

    auto do_command = std::make_unique<image::filter::FilterCommand>(
        receiver, std::move(filter));

    auto undo_command =
        std::make_unique<image::filter::RemoveFilterCommand>(receiver);

    command_history.executeCommand(std::move(do_command),
                                   std::move(undo_command));
  }
}

void renderUndoButton(image::history::CommandHistory &command_history,
                      float button_width) {
  if (!command_history.canUndo()) {
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    ImGui::Button(kButtonUndo, ImVec2(button_width, 0));
    ImGui::PopStyleVar();
  } else if (ImGui::Button(kButtonUndo, ImVec2(button_width, 0))) {
    command_history.Undo();
  }
}

void renderRedoButton(image::history::CommandHistory &command_history,
                      float button_width) {
  if (!command_history.canRedo()) {
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    ImGui::Button(kButtonRedo, ImVec2(button_width, 0));
    ImGui::PopStyleVar();
  } else if (ImGui::Button(kButtonRedo, ImVec2(button_width, 0))) {
    command_history.Redo();
  }
}

void renderBlurWithSlider(
    image::process::ImageProcessorManager &image_processor_manager,
    image::history::CommandHistory &command_history, int &blur_intensity,
    utils::AppContext &app_context) {

  ImGui::Text("Blur Intensity:");
  if (ImGui::SliderInt("##BlurIntensity", &blur_intensity, 1, 11)) {
    // Ensure odd numbers only - required for OpenCV kernel size
    if (blur_intensity % 2 == 0) {
      blur_intensity += 1;
    }
  }

  if (ImGui::Button("Apply Blur",
                    ImVec2(ImGui::GetContentRegionAvail().x, 0))) {
    auto receiver = std::make_shared<image::filter::FilterCommandReceiver>(
        &image_processor_manager);

    auto filter = std::make_unique<image::decorator::BlurDecorator>(
        std::make_unique<image::BaseImageProcessor>(), blur_intensity);

    auto do_command = std::make_unique<image::filter::FilterCommand>(
        receiver, std::move(filter));

    auto undo_command =
        std::make_unique<image::filter::RemoveFilterCommand>(receiver);

    command_history.executeCommand(std::move(do_command),
                                   std::move(undo_command));
  }
}

void renderRegionSelectionMenu(utils::AppContext &app_context) {
  if (!app_context.region_selector_ptr) {
    return;
  }

  auto mode = app_context.region_selector_ptr->GetMode();
  if (ImGui::RadioButton(
          "Full screen", mode == image::region::FilterMode::kFullScreen)) {
    if (mode != image::region::FilterMode::kFullScreen) {
      app_context.region_selector_ptr->ToggleMode();
    }
  }
  ImGui::SameLine();
  if (ImGui::RadioButton(
          "Selected region",
          mode == image::region::FilterMode::kPartialRegion)) {
    if (mode != image::region::FilterMode::kPartialRegion) {
      app_context.region_selector_ptr->ToggleMode();
    }
  }

  ImGui::Separator();
}

void renderFilterListItem(const std::string& filter_name, size_t index, 
                         image::process::ImageProcessorManager& manager,
                         image::history::CommandHistory& command_history) {
  ImGui::PushID(static_cast<int>(index));
  
  // Toggle switch
  bool enabled = manager.IsFilterEnabled(index);
  if (ImGui::Checkbox("##enabled", &enabled)) {
    manager.ToggleFilter(index);
  }
  
  ImGui::SameLine();
  
  // Filter name
  if (enabled) {
    ImGui::Text("%s", filter_name.c_str());
  } else {
    ImGui::TextDisabled("%s (disabled)", filter_name.c_str());
  }
  
  ImGui::SameLine();
  
  // Remove button
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 0.8f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
  if (ImGui::SmallButton("X")) {
    manager.RemoveFilter(index);
  }
  ImGui::PopStyleColor(2);
  
  ImGui::PopID();
}

void renderActiveFiltersList(image::process::ImageProcessorManager& manager,
                            image::history::CommandHistory& command_history) {
  if (!manager.HasActiveFilters()) {
    return;
  }

  ImGui::Separator();
  ImGui::Text("Applied effects:");

  auto filter_names = manager.GetActiveFilters();
  for (size_t i = 0; i < filter_names.size(); i++) {
    renderFilterListItem(filter_names[i], i, manager, command_history);
  }
}

void renderEffectsMenu(utils::AppContext &app_context, bool is_grid_view) {
  if (is_grid_view) {
    return;
  }

  ImGui::Text("Effects");
  ImGui::Separator();

  renderRegionSelectionMenu(app_context);

  std::optional<int> selected_camera_id{};
  image::history::CommandHistory *active_command_history = nullptr;

  if (app_context.cameras_ptr && !app_context.cameras_ptr->empty()) {
    if (app_context.current_id_ptr && *app_context.current_id_ptr >= 0) {
      selected_camera_id = *app_context.current_id_ptr;
    }

    if (selected_camera_id.has_value()) {
      int target_camera_id = selected_camera_id.value();

      for (auto &camera : *app_context.cameras_ptr) {
        if (camera.id == target_camera_id && camera.processor_manager &&
            camera.command_history) {
          active_command_history = camera.command_history.get();
          ImGui::Text("Applying effects to Camera %d", camera.id);

          ImGui::Text("Add effects:");
          gui::renderFilterButton<image::decorator::GrayscaleDecorator>(
              kButtonSetGrayscale, *camera.processor_manager,
              *active_command_history, app_context);
          gui::renderBlurWithSlider(*camera.processor_manager,
                                    *active_command_history,
                                    app_context.blur_intensity, app_context);
          gui::renderFilterButton<image::decorator::SepiaDecorator>(
              kButtonSetSepia, *camera.processor_manager,
              *active_command_history, app_context);
          gui::renderFilterButton<image::decorator::EdgeDetectionDecorator>(
              kButtonSetEdgeDetection, *camera.processor_manager,
              *active_command_history, app_context);

          if (camera.processor_manager->HasActiveFilters()) {
            renderActiveFiltersList(*camera.processor_manager, *active_command_history);
          }
          break;
        }
      }
    }
  } else if (app_context.image_processor_manager_ptr &&
             app_context.command_history_ptr) {
    active_command_history = app_context.command_history_ptr.get();
    auto &image_processor_manager = *app_context.image_processor_manager_ptr;

    ImGui::Text("Add effects:");
    gui::renderFilterButton<image::decorator::GrayscaleDecorator>(
        kButtonSetGrayscale, image_processor_manager, *active_command_history,
        app_context);
    gui::renderBlurWithSlider(image_processor_manager, *active_command_history,
                              app_context.blur_intensity, app_context);
    gui::renderFilterButton<image::decorator::SepiaDecorator>(
        kButtonSetSepia, image_processor_manager, *active_command_history,
        app_context);
    gui::renderFilterButton<image::decorator::EdgeDetectionDecorator>(
        kButtonSetEdgeDetection, image_processor_manager,
        *active_command_history, app_context);

    if (image_processor_manager.HasActiveFilters()) {
      renderActiveFiltersList(image_processor_manager, *active_command_history);
    }
  }

  if (active_command_history) {
    ImGui::Separator();

    float half_button_width = ImGui::GetContentRegionAvail().x * 0.5f -
                              ImGui::GetStyle().ItemSpacing.x * 0.5f;
    if (half_button_width < 0.0f) {
      half_button_width = 0.0f;
    }

    gui::renderUndoButton(*active_command_history, half_button_width);
    ImGui::SameLine();
    gui::renderRedoButton(*active_command_history, half_button_width);
  }
}

} // namespace

/// @brief Renders the menu bar with options for exiting the application and
/// updating the camera list.
/// @param window Pointer to the GLFW window.
/// @param app_context Application context containing camera and state
/// information.
/// @param grid_display Grid display for rendering camera data.
void renderMenuBar(GLFWwindow *window, utils::AppContext &app_context,
                   GridDisplay &grid_display) {
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu(kApplicationName)) {
      renderCampoMenu(window);
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Camera")) {
      renderCameraMenu(app_context, grid_display);
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}

/// @brief Renders detailed information about available cameras.
/// @param cameras A vector of CameraData objects containing camera details.
void renderCameraDetails(const std::vector<utils::CameraData> &cameras) {
  if (!cameras.empty()) {
    for (const auto &camera : cameras) {
      if (camera.is_available) {
        cameraDetails(camera);
        ImGui::Separator();
      }
    }
  } else {
    ImGui::Text("No cameras available.");
  }
}

/// @brief Renders the left panel of the GUI, including the menu bar and camera
/// details.
/// @param window Pointer to the GLFW window.
/// @param app_context Application context containing camera and state
/// information.
/// @param grid_display Grid display for rendering camera data.
void renderLeftPanel(GLFWwindow *window, utils::AppContext &app_context,
                     GridDisplay &grid_display) {
  ImGui::BeginChild("Left Panel",
                    ImVec2(ImGui::GetContentRegionAvail().x * 0.25f, 0), true,
                    ImGuiWindowFlags_MenuBar);

  renderMenuBar(window, app_context, grid_display);

  ImGui::Separator();

  if (!app_context.cameras_ptr->empty()) {
    renderCameraDetails(*app_context.cameras_ptr);
  } else {
    ImGui::Text("No cameras available.");
  }

  renderEffectsMenu(app_context, grid_display.IsGridView());

  ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 32);
  ImGui::Separator();
  grid_display.RenderReturnButton();

  ImGui::EndChild();
}

/// @brief Renders the right panel of the GUI, including the camera grid
/// display.
/// @param grid_display Grid display for rendering camera data.
/// @param current_id Reference to the current camera ID.
void renderRightPanel(GridDisplay &grid_display, int &current_id,
                      utils::AppContext &app_context) {
  ImGui::BeginChild("Right Panel", ImVec2(0, 0), true);

  std::optional<int> chosen_camera =
      grid_display.RenderGrid(app_context.region_selector_ptr.get());
  if (chosen_camera.has_value()) {
    current_id = chosen_camera.value();
  }

  ImGui::EndChild();
}

/// @brief Initializes a new frame for rendering the GUI.
void initNewFrame() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

/// @brief Renders the entire GUI, including both left and right panels.
/// @param window Pointer to the GLFW window.
/// @param app_context Application context containing camera and state
/// information.
/// @param grid_display Grid display for rendering camera data.
void renderGui(GLFWwindow *window, utils::AppContext &app_context,
               GridDisplay &grid_display) {
  initNewFrame();

  utils::Frame frame{};
  glfwGetFramebufferSize(window, &frame.width, &frame.height);

  const auto main_window_size{ImVec2(frame.width, frame.height)};
  const auto main_window_pos{ImVec2(kWindowPosX, 0)};

  ImGui::SetNextWindowSize(main_window_size);
  ImGui::SetNextWindowPos(main_window_pos);

  if (ImGui::Begin(kApplicationName, NULL,
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                       ImGuiWindowFlags_NoMove)) {
    renderLeftPanel(window, app_context, grid_display);

    ImGui::SameLine();

    renderRightPanel(grid_display, *app_context.current_id_ptr, app_context);
    ImGui::End();
  }

  ImGui::Render();
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace gui
