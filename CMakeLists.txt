cmake_minimum_required(VERSION 3.5)
project(Mini_RPG)

include_directories(source)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES Makefile README.md .circleci resources source test source/Model/Tools/Tool.cc source/ConfigReader.cc source/Model/Sprites/Prop.cc)

add_executable(Mini_RPG ${SOURCE_FILES} ${HEADER_FILES} source/main.cc)
