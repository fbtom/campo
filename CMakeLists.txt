cmake_minimum_required(VERSION 3.31)
project(Campo VERSION 0.01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(utils/utils.cmake)

add_executable(${PROJECT_NAME}
  main.cpp
)

add_subdirectory(external)

find_package(OpenGL)

add_library(ImGui STATIC)

target_open_cv(${PROJECT_NAME})

set(ImGui_SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/imgui-src")
target_include_directories(ImGui PUBLIC
    ${ImGui_SOURCE_DIR}
    ${ImGui_SOURCE_DIR}/backends
    ${GLFW_SOURCE_DIR}/include
)

target_sources(ImGui PRIVATE
    ${ImGui_SOURCE_DIR}/imgui.cpp
    ${ImGui_SOURCE_DIR}/imgui_demo.cpp
    ${ImGui_SOURCE_DIR}/imgui_draw.cpp
    ${ImGui_SOURCE_DIR}/imgui_tables.cpp
    ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
    ${ImGui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    ${ImGui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw ImGui OpenGL::GL nlohmann_json::nlohmann_json)

add_subdirectory(test)
