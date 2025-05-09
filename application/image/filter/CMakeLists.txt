add_library(image_filter STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/filter_command.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filter_command.hpp
)

target_include_directories(image_filter PUBLIC
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(image_filter PUBLIC image_decorator)
