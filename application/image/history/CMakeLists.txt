add_library(image_history INTERFACE)
target_sources(image_history INTERFACE   
                                    ${CMAKE_CURRENT_SOURCE_DIR}/command.hpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/command_history.hpp
                                )

target_include_directories(image_history INTERFACE 
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(image_history INTERFACE ImGui image_filter image_process)

target_open_cv(image_history)
