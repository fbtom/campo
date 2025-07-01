add_library(image_region INTERFACE)

target_sources(image_region INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/region_selector.hpp
)

target_include_directories(image_region INTERFACE 
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(image_region INTERFACE ImGui)

target_open_cv(image_region)
