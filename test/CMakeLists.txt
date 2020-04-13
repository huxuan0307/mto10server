# 最低版本号要求
cmake_minimum_required(VERSION 2.8)
# 项目信息
project(test)

add_executable(regex_test regex_test.cpp)
add_executable(distro_test distro_test.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

