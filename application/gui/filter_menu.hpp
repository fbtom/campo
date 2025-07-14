///
/// @file filter_menu.hpp
/// @author fbtom
/// @brief 
/// @date 2025-07-14
///
/// @copyright Copyright (c) 2025
///

#pragma once

#include "application/image/decorator/filter_decorators.hpp"
#include "application/image/filter/filter_command.hpp"
#include "application/image/filter/filter_command_receiver.hpp"
#include "application/image/filter/remove_filter_command.hpp"
#include "application/image/history/command_history.hpp"
#include "application/image/image_process/image_processor_manager.hpp"
#include "gui_utils.hpp"
#include "shortcuts.hpp"
#include "utils/callback_handler.hpp"
#include <imgui.h>
#include <memory>
#include <optional>
#include <string>

namespace gui {

using FilterMode = image::region::FilterMode;

namespace filter_menu {

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
            FilterMode::kPartialRegion &&
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

  if (ImGui::Button(kButtonSetBlur,
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

void modeSelectionButton(const std::string &label,
                         utils::AppContext &app_context, bool screen_mode_0,
                         bool screen_mode_1) {
  if (ImGui::RadioButton(label.c_str(), screen_mode_0)) {
    if (screen_mode_1) {
      app_context.region_selector_ptr->ToggleMode();
    }
  }
}

void renderRegionSelectionMenu(utils::AppContext &app_context) {
  if (!app_context.region_selector_ptr) {
    return;
  }
  auto mode = app_context.region_selector_ptr->GetMode();
  const auto is_full = mode == FilterMode::kFullScreen;
  const auto is_partial = mode == FilterMode::kPartialRegion;

  modeSelectionButton("Full screen", app_context, is_full, is_partial);
  ImGui::SameLine();
  modeSelectionButton("Region", app_context, is_partial, is_full);

  ImGui::Separator();
}

void renderFilterListItem(const std::string &filter_name, size_t index,
                          image::process::ImageProcessorManager &manager,
                          image::history::CommandHistory &command_history) {
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

void renderActiveFiltersList(image::process::ImageProcessorManager &manager,
                             image::history::CommandHistory &command_history) {
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

void renderFilterButtons(image::process::ImageProcessorManager &manager,
                         image::history::CommandHistory &command_history,
                         utils::AppContext &app_context) {
  renderFilterButton<image::decorator::GrayscaleDecorator>(
      kButtonSetGrayscale, manager, command_history, app_context);

  renderBlurWithSlider(manager, command_history, app_context.blur_intensity,
                       app_context);

  renderFilterButton<image::decorator::SepiaDecorator>(
      kButtonSetSepia, manager, command_history, app_context);

  renderFilterButton<image::decorator::EdgeDetectionDecorator>(
      kButtonSetEdgeDetection, manager, command_history, app_context);

  if (manager.HasActiveFilters()) {
    renderActiveFiltersList(manager, command_history);
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

          renderFilterButtons(*camera.processor_manager,
                              *active_command_history, app_context);
          break;
        }
      }
    }
  } else if (app_context.image_processor_manager_ptr &&
             app_context.command_history_ptr) {
    active_command_history = app_context.command_history_ptr.get();
    auto &image_processor_manager = *app_context.image_processor_manager_ptr;

    renderFilterButtons(image_processor_manager, *active_command_history,
                        app_context);
  }

  if (active_command_history) {
    ImGui::Separator();

    float half_button_width = ImGui::GetContentRegionAvail().x * 0.5f -
                              ImGui::GetStyle().ItemSpacing.x * 0.5f;
    if (half_button_width < 0.0f) {
      half_button_width = 0.0f;
    }

    renderUndoButton(*active_command_history, half_button_width);
    ImGui::SameLine();
    renderRedoButton(*active_command_history, half_button_width);
  }
}

} // namespace filter_menu
} // namespace gui
