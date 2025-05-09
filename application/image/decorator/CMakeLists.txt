add_library(image_decorator INTERFACE)
target_sources(image_decorator INTERFACE   
                                ${CMAKE_CURRENT_SOURCE_DIR}/filter_decorators.hpp)

target_include_directories(image_decorator INTERFACE 
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(image_decorator INTERFACE ImGui image_process)

target_open_cv(image_decorator)
