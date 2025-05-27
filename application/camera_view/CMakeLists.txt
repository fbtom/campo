cmake_minimum_required(VERSION 3.14)

set(CAMERA_VIEW_SOURCES
    camera_utils.cpp
    camera_grid_view.cpp
    camera_single_view.cpp
    camera_view_controller.cpp
    camera_view_adapter.cpp
)

add_library(camera_view STATIC ${CAMERA_VIEW_SOURCES})

target_include_directories(camera_view
    PUBLIC
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(camera_view
    PUBLIC
        opencv_core
        opencv_imgproc
        glfw
        ImGui
        common
)
