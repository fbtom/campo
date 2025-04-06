///
/// @file camera_utils.hpp
/// @author fbtom 
/// @brief 
/// @date 2025-04-06
/// 
/// @copyright Copyright (c) 2025
/// 

#include <cstdio>
#include <vector>

auto collectCameraIDs()
{
  FILE* system_stderr = stderr;
#ifdef _WIN32
  FILE* null_stream = freopen("NUL", "w", stderr);
#else
  FILE* null_stream = freopen("/dev/null", "w", stderr);
#endif

  std::vector<uint8_t> camera_ids{};
  size_t max_camera_id = 50;
  for(size_t i = 0; i < max_camera_id; ++i) 
  {
    cv::VideoCapture cam(i);
    if(cam.isOpened()) 
    {
      camera_ids.push_back(i);
      cam.release();
    }
  }

  fflush(stderr);
  stderr = system_stderr;
  
  return camera_ids;
}