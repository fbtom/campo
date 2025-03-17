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

FetchContent_MakeAvailable(
  ImGui
  GTest
  OpenCV
)

