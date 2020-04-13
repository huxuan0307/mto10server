# 最低版本号要求
cmake_minimum_required(VERSION 2.8)
# 项目信息
project(mto10server)

add_subdirectory(src)
add_subdirectory(TCP)
aux_source_directory(. DIR_SRCS)
include_directories(./include)
include_directories(./TCP)
include_directories(.)
link_directories(/usr/lib64/mysql)

add_executable(mto10server main.cpp)
target_link_libraries(mto10server md5 database tools mysqlclient server TcpServer crypto)

add_executable(mto10_read mto10_read.cpp)
target_link_libraries(mto10_read mysqlclient tools)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

