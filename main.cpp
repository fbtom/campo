///
/// @file main.cpp
/// @author fbtom
/// @brief
/// @date 2025-03-07
///
/// @copyright Copyright (c) 2025
///

// System headers
#include <iostream>
// External headers
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "opencv2/opencv.hpp"
#include <GLFW/glfw3.h>
// Project headers
#include "utils/camera.hpp"
#include "utils/json.hpp"

using std::cerr;

void initWindowHint()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLuint cvmatToTexture(cv::Mat &mat)
{
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);

  cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return textureId;
}

int main()
{
  // Init Phase
  if (!glfwInit())
  {
    return -1;
  }

  initWindowHint();

  GLFWwindow *window = glfwCreateWindow(800, 600, "Campo", NULL, NULL);
  glfwMakeContextCurrent(window);

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");

  const auto camera_ids = utils::getCameraIDs();
  auto current_id = utils::getValidCameraID(camera_ids, utils::loadCameraID());

  cv::VideoCapture cam(current_id);

  // Main Phase
  cv::Mat frame{};
  GLuint textureId = 0;

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    cam >> frame;

    if (frame.empty())
    {
      break;
    }
    textureId = cvmatToTexture(frame);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Camera Preview");
    if (textureId)
    {
      ImGui::Image((ImTextureID)(uintptr_t)textureId, ImVec2(frame.cols, frame.rows));
    }
    ImGui::End();

    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  utils::saveCameraID(current_id);

  // Shutdown Phase
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
  }

  return 0;
}
