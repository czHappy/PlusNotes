# CMake 最低版本要求
cmake_minimum_required(VERSION 3.0)

# 项目名称
project(MyProject)

# 构建库
add_library(MyLibrary SHARED my_library.cpp)

# 构建可执行文件并链接库
add_executable(MyApp main.cpp)
target_link_libraries(MyApp MyLibrary)
