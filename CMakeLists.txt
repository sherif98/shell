cmake_minimum_required(VERSION 3.6)
project(shell)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES history.h history.c main.c string_utilities.h string_utilities.c)
add_executable(shell ${SOURCE_FILES})