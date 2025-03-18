#include <iostream>
#include "opencv2/opencv.hpp"

using std::cerr;

int main() 
{
  cv::VideoCapture cam{0};
  if(!cam.isOpened())
  {
    cerr << "Cannot open Your camera\n";
  }
  cv::Mat frame{};
  while(cv::waitKey(1) != 27)
  {
    cam >> frame; 
    if(frame.empty()) 
    {
      break;
    }

    cv::imshow("Preview", frame);
  }
  return 0;
}
