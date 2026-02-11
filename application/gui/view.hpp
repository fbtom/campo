///
/// @file view.hpp
/// @author fbtom
/// @brief
/// @date 2026-01-02
///
/// @copyright Copyright (c) 2026
///

#pragma once
#include <memory>

#include "glfw/utils.hpp"
#include "grid_display.hpp"
#include "gui_utils.hpp"
#include "menu.hpp"
#include "panel_renderer.hpp"
#include "utils/frame.hpp"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <unordered_map>

namespace gui {

enum class ViewType {
  Welcome,
  MainSingleCam,
  MainMultiCam,
  SingleCam,
};

class View {
protected:
  virtual void Render(GLFWwindow *window, utils::AppContext &app_context) = 0;

public:
  void RenderView(GLFWwindow *window, utils::AppContext &app_context) {
    initNewFrame();
    setWindowProperties(window);
    ImGui::Begin(kApplicationName, &app_context.is_running, getFlags());

    Render(window, app_context);
    ImGui::End();

    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }
  virtual ~View() = default;
};

static constexpr const char *ResolutionName[] = {"800x600", "1920x1080",
                                                 "1280x720", "640x480"};

class WelcomeView : public View {
public:
  void Render(GLFWwindow *window, utils::AppContext &app_context) override {
    const auto parent_size = ImGui::GetContentRegionAvail();
    const auto sidebar_width = parent_size.x * .25f;
    const auto tab_view_width =
        parent_size.x - sidebar_width - ImGui::GetStyle().ItemSpacing.x;

    ImGui::BeginChild("##SideBar", ImVec2(sidebar_width, parent_size.y), true);
    {
      const auto parent_size{ImGui::GetContentRegionAvail()};
      const auto menu_width{parent_size.x * .8f};
      ImVec2 box_size{menu_width, 400.f};
      const auto start_pos_menu_x{(parent_size.x - box_size.x) * 0.5f};
      const auto start_pos_menu_y{(parent_size.y - box_size.y) * 0.5f};
      ImGui::SetCursorPos(ImVec2(start_pos_menu_x, start_pos_menu_y));
      if (app_context.gui_context.menu == utils::MenuState::DEFAULT) {

        ImGui::BeginChild("##Menu", box_size, true);
        {
          ImGui::Text("Hello User!{%d}",
                      static_cast<int>(app_context.gui_context.menu));
          if (ImGui::Button("Settings")) {
            app_context.gui_context.menu = utils::MenuState::SETTINGS;
          }
          if (ImGui::Button("SwapView")) {
            app_context.gui_context.current_view =
                utils::Views::MAINSINGLECAMVIEW;
          }
          if (ImGui::Button("Exit")) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
          }
        }
        ImGui::EndChild();
      } else if (app_context.gui_context.menu == utils::MenuState::SETTINGS) {
        ImGui::BeginChild("##Settings", box_size, true);
        // TODO center Settings text box
        ImGui::Text("Settings");
        {

          static ImGuiComboFlags flags{ImGuiComboFlags_NoArrowButton};
          const auto &current = toIndex(app_context.gui_context.window_size);
          const auto longest_text_size{ImGui::CalcTextSize(
              ResolutionName[toIndex(utils::Resolution::R1920X1080)])};
          const auto combo_width{longest_text_size.x +
                                 ImGui::GetStyle().FramePadding.x * 2};
          const auto combo_height{longest_text_size.y +
                                  ImGui::GetStyle().FramePadding.y * 2};
          if (ImGui::BeginTable("Menu", 3,
                                ImGuiTableFlags_BordersInner |
                                    ImGuiTableFlags_BordersOuter)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%s", "Resolution");

            ImGui::TableSetColumnIndex(1);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%s", ResolutionName[current]);
            ImGui::TableSetColumnIndex(2);

            ImGui::SetNextItemWidth(combo_width);
            // TODO make temportary state - when APPLY pressed
            // Make temporary state become current state
            if (ImGui::BeginCombo("##Resolution", ResolutionName[current],
                                  flags)) {
              const auto size{toIndex(utils::Resolution::SIZE)};
              for (int i{0}; i < size; ++i) {
                bool selected = (current == i);
                if (ImGui::Selectable(ResolutionName[i], selected)) {
                  app_context.gui_context.window_size = utils::toEnum(i);
                }
                if (selected) {
                  ImGui::SetItemDefaultFocus();
                }
              }
              ImGui::EndCombo();
            };

            ImGui::TableNextRow();
            // TODO create logic to swap between visual modes
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", "Mode");
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", "Dark Mode");
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", "Light Mode");

            ImGui::EndTable();
          }

          if (ImGui::BeginTable("SettingsBttns", 2)) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            if (ImGui::Button("Back")) {
              app_context.gui_context.menu = utils::MenuState::DEFAULT;
            }
            ImGui::TableSetColumnIndex(1);
            const auto apply_width{ImGui::CalcTextSize("Apply").x +
                                   ImGui::GetStyle().FramePadding.x * 2};
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
                                 ImGui::GetColumnWidth() - apply_width);
            // TODO create logic to apply desired_state to current
            // TODO Apply should appear only when any desired state != current
            // state
            const auto disabled = true;
            ImGui::BeginDisabled(disabled);
            if (ImGui::Button("Apply")) {
            }
            ImGui::EndDisabled();
            ImGui::EndTable();
          }
        }
        ImGui::EndChild();
      }
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Main", ImVec2(tab_view_width, parent_size.y), false);
    {
      const auto parent_size{ImGui::GetContentRegionAvail()};
      const float spacing = ImGui::GetStyle().ItemSpacing.y;
      const float height = parent_size.y - 2.0f * spacing;
      const auto view_height{height * 0.5f};
      const ImVec2 view_size{parent_size.x, view_height};
      // ImGui::InvisibleButton("##ManyCam View Clickable", view_size);
      // if (ImGui::IsItemClicked()) {
      //   // TODO set viewType to MainMultiCam
      // }
      ImGui::BeginChild("ManyCam View", view_size, true);
      {
        static int value{0};
        if (ImGui::IsWindowHovered() &&
            ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
          // TODO change view to Many CamView
          value++;
        }
        const auto parent_size{ImGui::GetContentRegionAvail()};
        const auto spacing = 8.f;
        const auto max_grid_width = parent_size.x;
        const auto max_grid_height = parent_size.y;

        auto child_width = (max_grid_width - spacing) / 2.f;
        auto child_height = child_width * 9.f / 16.f;
        if ((child_height * 2.f + spacing) > max_grid_height) {
          child_height = ((max_grid_height - spacing) * .5f);
          child_width = child_height * (16.f / 9.f);
        }
        const ImVec2 view_size{child_width, child_height};
        const ImVec2 grid_size{child_width * 2.f + spacing,
                               child_height * 2.f + spacing};
        const auto cursor = ImGui::GetCursorPos();
        ImGui::SetCursorPos({cursor.x + (parent_size.x - grid_size.x) * .5f,
                             cursor.y + (parent_size.y - grid_size.y) * .5f});
        ImGui::BeginChild("Grid", grid_size, false, ImGuiWindowFlags_NoInputs);
        {
          ImGui::BeginChild("##FirstView", view_size, true,
                            ImGuiWindowFlags_NoInputs);
          // TODO instead of text insert scaled views from connected cameras
          ImGui::Text("%s", "text");
          ImGui::Text("%d", value);
          ImGui::EndChild();
          ImGui::SameLine(.0f, spacing);
          ImGui::BeginChild("##SecondView", view_size, true,
                            ImGuiWindowFlags_NoInputs);
          ImGui::Text("%s", "text");

          ImGui::EndChild();
          ImGui::Dummy({0.f, 0.f});
          ImGui::BeginChild("##ThirdView", view_size, true,
                            ImGuiWindowFlags_NoInputs);
          ImGui::Text("%s", "text");

          ImGui::EndChild();
          ImGui::SameLine(.0f, spacing);
          ImGui::BeginChild("##FourthView", view_size, true,
                            ImGuiWindowFlags_NoInputs);
          ImGui::Text("%s", "text");

          ImGui::EndChild();
        }
        ImGui::EndChild();
      }
      ImGui::EndChild();

      ImGui::BeginChild("SingleCam View", view_size, true);
      {
        static int value{0};
        if (ImGui::IsWindowHovered() &&
            ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
          // TODO change view to Single CamView
          value++;
        }
        const auto parent_size{ImGui::GetContentRegionAvail()};
        const auto spacing = 8.f;
        const auto max_grid_width = parent_size.x;
        const auto max_grid_height = parent_size.y;

        auto child_width = (max_grid_width - spacing);
        auto child_height = child_width * 9.f / 16.f;
        if ((child_height + spacing) > max_grid_height) {
          child_height = ((max_grid_height - spacing));
          child_width = child_height * (16.f / 9.f);
        }
        const ImVec2 grid_size{child_width + spacing, child_height + spacing};
        const auto cursor = ImGui::GetCursorPos();
        ImGui::SetCursorPos({cursor.x + (parent_size.x - grid_size.x) * .5f,
                             cursor.y + (parent_size.y - grid_size.y) * .5f});
        ImGui::BeginChild("##SingleView", grid_size, true,
                          ImGuiWindowFlags_NoInputs);
        // TODO instead of text insert scaled views from connected cameras

        ImGui::Text("%d", value);
        ImGui::Text("%s", "text");

        ImGui::EndChild();
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();
  }
};

class MainSingleCamView : public View {
public:
  void Render(GLFWwindow *window, utils::AppContext &app_context) override {
    const auto parent_size = ImGui::GetContentRegionAvail();
    // TODO learn about padding and apply it correctly
    const auto width = parent_size.x * .95f - ImGui::GetStyle().ItemSpacing.x;
    const auto height = parent_size.y * .95f - ImGui::GetStyle().ItemSpacing.y;
    const auto start_pos_menu_x{(parent_size.x - width) * 0.5f};
    const auto start_pos_menu_y{(parent_size.y - height) * 0.5f};
    ImGui::SetCursorPos(ImVec2(start_pos_menu_x, start_pos_menu_y));

    ImGui::BeginChild("Main", {width, height}, ImGuiChildFlags_Borders,
                      ImGuiWindowFlags_NoScrollbar |
                          ImGuiWindowFlags_NoScrollWithMouse);
    {
      const auto parent_size = ImGui::GetContentRegionAvail();
      const auto width = parent_size.x;
      const auto text_size = ImGui::CalcTextSize("Current Cam");
      const auto height =
          parent_size.y - text_size.y - ImGui::GetStyle().ItemSpacing.y * 2;

      ImGui::BeginChild("Current Cam", {width, height}, ImGuiChildFlags_Borders,
                        ImGuiWindowFlags_NoScrollbar |
                            ImGuiWindowFlags_NoScrollWithMouse);

      // const auto parent_size{ImGui::GetContentRegionAvail()};
      // const auto current {
      //   app_context.ImGui::Image(
      //       static_cast<ImTextureID>(app_context.current_id_ptr), );
      // }
      ImGui::EndChild();

      ImGui::BeginChild(
          "Footer",
          {parent_size.x, parent_size.y - ImGui::GetStyle().ItemSpacing.y * 2});
      {
        ImGui::Text("%s", "Cam View");
        ImGui::SameLine();
        // TODO keep state of which camera is currentply on
        ImGui::Text("%s", "Cam ID X");
        const auto parent_size{ImGui::GetContentRegionAvail()};
        const auto arrow_left{ImGui::CalcTextSize("<-")};
        const auto arrow_right{ImGui::CalcTextSize("->")};
        const auto padding{ImGui::GetStyle().FramePadding};
        const auto placement{parent_size.x - arrow_left.x - arrow_right.x -
                             padding.x * 6};

        ImGui::SameLine();
        ImGui::SetCursorPosX(placement);
        ImGui::Button("<-");
        ImGui::SameLine();
        ImGui::Button("->");
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();
  }
};

class MainMultiCamView : public View {
public:
  void Render(GLFWwindow *window, utils::AppContext &app_context) override {}
};

class SingleCamView : public View {
public:
  void Render(GLFWwindow *window, utils::AppContext &app_context) override {
    ImGui::BeginChild("asdf", {});
    ImGui::EndChild();
  }
};

// pseudo code

std::unique_ptr<View> getView(utils::Views view) {
  switch (view) {
  case utils::Views::WELCOMEVIEW:
    return std::make_unique<WelcomeView>();
    break;
  case utils::Views::MAINSINGLECAMVIEW:
    return std::make_unique<MainSingleCamView>();
    break;
  case utils::Views::MAINMULTICAMVIEW:
    return std::make_unique<MainMultiCamView>();
    break;
  case utils::Views::SINGLECAMVIEW:
    return std::make_unique<SingleCamView>();
    break;
  default:
    return std::make_unique<WelcomeView>();
    break;
  }
}

class Context {
  std::unique_ptr<View> currentView;
  utils::Views previous_view{utils::Views::WELCOMEVIEW};

public:
  Context() : currentView(std::make_unique<WelcomeView>()) {}

  void Render(GLFWwindow *window, utils::AppContext &app_context) {
    if (previous_view != app_context.gui_context.current_view) {
      previous_view = app_context.gui_context.current_view;
      currentView = getView(previous_view);
    }
    currentView->RenderView(window, app_context);
  }
};

}; // namespace gui
