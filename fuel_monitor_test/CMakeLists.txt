cmake_minimum_required(VERSION 2.8.3)
project(fuel_monitor)

# # GFlags
# find_package(Gflags REQUIRED)
# message(STATUS "GFLAGS_LIBRARIES: ${GFLAGS_LIBRARIES}")

# jsoncpp
find_package(JsonCpp REQUIRED)
include_directories(${JsonCpp_INCLUDE_DIRS})
message(STATUS "JsonCpp_INCLUDE_DIRS: ${JsonCpp_INCLUDE_DIRS}")
message(STATUS "JsonCpp_LIBRARIES: ${JsonCpp_LIBRARIES}")

# # GTest
# # import gtest after catkin, see https://plusai.slack.com/archives/C2LD3K1L5/p1611622999024200
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})
# message(STATUS "GTEST_LIBRARIES: ${GTEST_LIBRARIES}")

add_executable(fuel_monitor
    main.cpp
    fuel_monitor.cpp
)
target_link_libraries(fuel_monitor
    # ${GFLAGS_LIBRARIES}
    ${JsonCpp_LIBRARIES}
    # ${GTEST_LIBRARIES}
)