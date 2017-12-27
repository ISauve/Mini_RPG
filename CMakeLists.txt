cmake_minimum_required(VERSION 3.5)
project(Mini_RPG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES Makefile README.md .circleci src test assets)

add_executable(Mini_RPG ${SOURCE_FILES} ${HEADER_FILES} src/main.cc)
