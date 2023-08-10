cmake_minimum_required(VERSION 3.0)
project(ReadImageInfo)

find_package(OpenCV REQUIRED)

add_executable(ReadImageInfo main.cpp)
target_link_libraries(ReadImageInfo ${OpenCV_LIBS})
