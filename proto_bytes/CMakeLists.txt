cmake_minimum_required(VERSION 3.5)
project(ProtobufExample)

# 使用 find_package 找到 Protobuf
find_package(Protobuf REQUIRED)

# 添加生成的 .proto 文件和相关 C++ 文件到项目中
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS your_message.proto)

# 添加可执行目标
add_executable(main main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# 链接 Protobuf 库
target_link_libraries(main protobuf)
