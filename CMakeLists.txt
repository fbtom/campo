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
endforeach()

foreach(module ${OpenCVModules})
    target_link_libraries(${PROJECT_NAME} PRIVATE opencv_${module})
endforeach()

