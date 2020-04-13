cmake_minimum_required(VERSION 2.8)

project(mto10server)
include_directories(../include)
aux_source_directory(. DIR_LIB_SRCS)
add_library(md5 md5.cpp)
add_library(tools read_config.cpp logging.cpp argument.cpp )
add_library(database database.cpp)
add_library(server mto10server.cpp mto10session.cpp)

target_link_libraries(server database tools)
# add_library(Functions ${DIR_LIB_SRCS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

