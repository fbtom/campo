set(COMMON_HEADERS
    camera_stream.hpp
)

add_library(common INTERFACE)

target_include_directories(common
    INTERFACE
        ${CMAKE_SOURCE_DIR}
)

target_sources(common
    INTERFACE
        ${COMMON_HEADERS}
)
