cmake_minimum_required(VERSION 2.8)

project(TcpServer)
include_directories(../include)
aux_source_directory(. DIR_LIB_SRCS)
# add_executable(TcpServer ${DIR_LIB_SRCS})
add_library(TcpServer ${DIR_LIB_SRCS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

