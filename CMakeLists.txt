cmake_minimum_required(VERSION 3.31)
project(Campo VERSION 0.01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
  main.cpp
)

add_subdirectory(external)

set(OpenCVModules
    core
    stitching
    video
    imgproc
    stereo
    photo
    highgui
    objdetect
    imgcodecs
    videoio
    dnn
    calib
    features
    flann
    3d
)

find_package(OpenGL)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OpenCV_SOURCE_DIR}/include
    ${OpenCV_BINARY_DIR}
    ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
    ${OPENCV_MODULE_opencv_modules_LOCATION}/include
)

foreach(module ${OpenCVModules})
    target_include_directories(${PROJECT_NAME} PRIVATE
        ${OPENCV_MODULE_opencv_${module}_LOCATION}/include
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE opencv_${module})
endforeach()

add_library(ImGui STATIC)

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

target_link_libraries(${PROJECT_NAME} PRIVATE glfw ImGui OpenGL::GL)

