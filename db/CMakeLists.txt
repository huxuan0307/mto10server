cmake_minimum_required(VERSION 2.8)
project(CreateRandomUsers)
link_directories(/usr/lib64/mysql)
link_directories(../src)
include_directories(../include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


add_executable(CreateRandomUsers CreateRandomUser.cpp md5.cpp)
add_executable(mto10_read mto10_read.cpp)
target_link_libraries(CreateRandomUsers mysqlclient crypto)
target_link_libraries(mto10_read mysqlclient tools)