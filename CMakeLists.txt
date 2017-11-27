cmake_minimum_required(VERSION 3.5)
project(Mini_RPG)

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES Makefile README.md resources src test bin .circleci)

add_executable(Mini_RPG ${SOURCE_FILES})
