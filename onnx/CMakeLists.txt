# cmake_minimum_required(VERSION 3.10)
# project(NvOnnxParserExample)

# # 查找 TensorRT 库和头文件
# find_path(TENSORRT_INCLUDE_DIR NvInfer.h HINTS /usr/include/x86_64-linux-gnu)
# find_library(TENSORRT_LIBRARY nvinfer HINTS /usr/lib/x86_64-linux-gnu)

# # 包含头文件目录
# include_directories(${TENSORRT_INCLUDE_DIR})

# # 设置 C++ 标准
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# # 添加可执行文件
# add_executable(NvOnnxParserExample main.cpp)

# # 链接 TensorRT 库
# target_link_libraries(NvOnnxParserExample ${TENSORRT_LIBRARY})

cmake_minimum_required(VERSION 3.10)

project(TensorRT_ONNX_Example)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 CUDA 包
find_package(CUDA REQUIRED)

# TensorRT 头文件路径和库路径
set(TensorRT_DIR "/usr/src/TensorRT-8.6.1.6")  # 替换为 TensorRT 安装路径
include_directories(${TensorRT_DIR}/include)
link_directories(${TensorRT_DIR}/lib)

include_directories("${CUDA_INCLUDE_DIRS}")
# 查找 cuDNN 包（可选）
# find_package(CUDNN REQUIRED) 

# 生成可执行文件
add_executable(tensorrt_onnx_example main.cpp)
add_executable(anonymous anonymous.cpp)
add_executable(engine engine.cpp)
# 链接 TensorRT 和 CUDA 库
target_link_libraries(tensorrt_onnx_example nvinfer nvonnxparser cudart)
target_link_libraries(anonymous nvinfer nvonnxparser cudart)
target_link_libraries(engine nvinfer nvonnxparser cudart)
# 如果需要 cuDNN，则取消以下注释
# target_link_libraries(tensorrt_onnx_example ${CUDNN_LIBRARIES})
