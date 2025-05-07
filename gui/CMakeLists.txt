add_library(gui INTERFACE)
target_sources(gui INTERFACE    ${CMAKE_CURRENT_SOURCE_DIR}/gui_utils.hpp 
                                ${CMAKE_CURRENT_SOURCE_DIR}/grid_display.hpp 
                                ${CMAKE_CURRENT_SOURCE_DIR}/menu.hpp)

target_include_directories(gui INTERFACE 
    ${CMAKE_SOURCE_DIR}
)
