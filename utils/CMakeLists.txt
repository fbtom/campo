add_library(utils STATIC) 
target_sources(utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/callback_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/frame.hpp
)

target_link_libraries(utils PUBLIC gui nlohmann_json::nlohmann_json glfw) 

target_include_directories(utils PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR} 
)

target_open_cv(utils)
