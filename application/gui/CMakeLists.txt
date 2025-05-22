add_library(gui INTERFACE)
target_sources(gui INTERFACE    ${CMAKE_CURRENT_SOURCE_DIR}/gui_utils.hpp 
                                ${CMAKE_CURRENT_SOURCE_DIR}/grid_display.hpp 
                                ${CMAKE_CURRENT_SOURCE_DIR}/shortcuts.hpp 
                                ${CMAKE_CURRENT_SOURCE_DIR}/menu.hpp)

target_include_directories(gui INTERFACE 
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(gui INTERFACE ImGui image_history image_filter image_decorator image_process camera_view common)
