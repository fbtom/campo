add_library(utils INTERFACE)
target_sources(utils INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/gui.hpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/json.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_utils.hpp
)

target_link_libraries(utils INTERFACE gui)

target_include_directories(utils INTERFACE 
    ${CMAKE_SOURCE_DIR}
)