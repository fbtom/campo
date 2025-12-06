add_library(detections INTERFACE)
target_sources(detections INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/detection_algorithms.hpp
)

target_include_directories(detections INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_open_cv(detections)
