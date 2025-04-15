include(FetchContent)

FetchContent_Declare(OpenCV
  GIT_REPOSITORY  https://github.com/opencv/opencv.git
  GIT_TAG         5.x
  GIT_PROGRESS    TRUE
  GIT_SHALLOW     TRUE 
  SYSTEM
)

FetchContent_Declare(GTest
  GIT_REPOSITORY  https://github.com/google/googletest.git
  GIT_TAG         v1.16.0
  SYSTEM
)

FetchContent_Declare(ImGui
  GIT_REPOSITORY  https://github.com/ocornut/imgui.git
  GIT_TAG         v1.91.8
  SYSTEM
)

FetchContent_Declare(GLFW
  GIT_REPOSITORY  https://github.com/glfw/glfw.git
  GIT_TAG         3.3.8
  SYSTEM
)

set(CMAKE_POLICY_DEFAULT_CMP0054 NEW) 
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW) 

FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY  https://github.com/nlohmann/json.git
  GIT_TAG         v3.11.3
  SYSTEM
)

FetchContent_MakeAvailable(
  ImGui
  GTest
  OpenCV
  GLFW
  nlohmann_json
)
