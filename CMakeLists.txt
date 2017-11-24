cmake_minimum_required(VERSION 3.5)
project(Mini_RPG)

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -framework OpenGL -lsfml-system -lsfml-window -lsfml-graphics")

set(SOURCE_FILES Makefile README.md
        src/main.cc
        src/Controller/Controller.cc src/Controller/Controller.h
        src/Model/Model.cc src/Model/Model.h src/Model/Character.cc src/Model/Character.h src/Model/Weapon.cc src/Model/Weapon.h src/Model/Sprite.cc src/Model/Sprite.h src/Model/Notification.h src/Model/EventPackage.h
        src/Observer/Subject.cc src/Observer/Subject.h src/Observer/Observer.cc src/Observer/Observer.h
        src/View/View.cc src/View/View.h src/Channel.h src/View/DrawingHelpers.cc src/View/DrawingThread.cc src/Model/GameThread.cc
        test/MyTestSample.h test/sample.cc)

add_executable(Mini_RPG ${SOURCE_FILES})
