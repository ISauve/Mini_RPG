cmake_minimum_required(VERSION 3.5)
project(Mini_RPG)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Makefile
        src/main.cpp
        src/Controller/Controller.cpp src/Controller/Controller.h
        src/Model/Model.cpp src/Model/Model.h src/Model/Character.cpp src/Model/Character.h src/Model/Weapon.cpp src/Model/Weapon.h  src/Model/Notification.h src/Model/Sprite.cpp src/Model/Sprite.h
        src/Observer/Subject.cpp src/Observer/Subject.h src/Observer/Observer.cpp src/Observer/Observer.h
        src/View/View.cpp src/View/View.h src/View/Channel.h src/View/DrawingHelpers.cpp src/View/DrawingThread.cpp)

add_executable(Mini_RPG ${SOURCE_FILES})
