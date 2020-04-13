# 项目简介

这是计算机网络课程设计的作业，使用C++语言实现的供“合成十”游戏服务端

# 基本架构

事务回调机制结合epoll的IO多路复用

# 目录介绍

## src 及 include 

包含项目的源码

## log 

存放运行时日志

## db

包含数据库的数据表以及数据库的测试用例

## TCP

事务回调机制结合epoll多路IO复用实现的TCP服务器

## test 

一些有关regex和random库的测试，GCC4.8的regex未实现完全(regex_search函数未实现)，由此测试发现问题。
