include(${CMAKE_SOURCE_DIR}/utils/utils.cmake)

enable_testing()

add_executable(camera_test
    utils/camera_test.cpp
)
target_open_cv(camera_test)

target_include_directories(camera_test PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(camera_test PRIVATE
    utils 
    GTest::gtest_main
    GTest::gmock
    nlohmann_json::nlohmann_json
    OpenGL::GL
    glfw
    ImGui
)

include(GoogleTest)
gtest_discover_tests(camera_test)
