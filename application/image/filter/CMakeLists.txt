add_library(image_filter STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/filter_command.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filter_command_receiver.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/remove_filter_command.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/add_filter_command.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/remove_specific_filter_command.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/toggle_filter_command.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/add_filter_at_index_command.hpp
)

target_include_directories(image_filter PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(image_filter PUBLIC image_decorator)
