cmake_minimum_required(VERSION 3.10)
project(BMPimage)
set(CMAKE_CXX_STANDARD 17)

add_executable(BMPimg src/main.cpp src/BMPImg.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /Zi /fsanitize=address") 