cmake_minimum_required(VERSION 3.10)

project(JsonReadWriteExample)

# 添加 JSONCpp 库的路径（根据您的设置）
# jsoncpp
find_package(JsonCpp REQUIRED)
include_directories(${JsonCpp_INCLUDE_DIRS})

# 设置编译标志
set(CMAKE_CXX_STANDARD 11)

# 添加可执行文件
add_executable(JsonReadWriteExample main.cpp)

# 链接 JSONCpp 库
target_link_libraries(JsonReadWriteExample PRIVATE ${JsonCpp_LIBRARIES})
