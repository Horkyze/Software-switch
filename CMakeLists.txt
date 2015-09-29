cmake_minimum_required(VERSION 3.3)
project(s_switch)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    functions.h
    main.c
    thread.c)

add_executable(s_switch ${SOURCE_FILES})