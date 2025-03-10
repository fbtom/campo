cmake_minimum_required(VERSION 3.31)
project(Campo VERSION 0.01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Campo 
  main.cpp
)

add_subdirectory(external)

