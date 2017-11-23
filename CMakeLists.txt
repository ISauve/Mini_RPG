cmake_minimum_required(VERSION 3.5)
project(Mini_RPG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Makefile Model/Model.cpp Model/Model.h View/View.cpp View/View.h Controller/Controller.cpp Controller/Controller.h Subject.cpp Subject.h Observer.cpp Observer.h Model/Character.cpp Model/Character.h Model/Tool.cpp Model/Tool.h Model/Weapon.cpp Model/Weapon.h View/Animation.cpp View/Animation.h Model/Notification.h Model/Sprite.cpp Model/Sprite.h)
add_executable(Mini_RPG ${SOURCE_FILES})