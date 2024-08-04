cmake_minimum_required(VERSION 3.28)
project(KeyStroke_Crafters)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(KeyStroke_Crafters
        HashTable.cpp
        httplib.h
        json.hpp
        main.cpp
        RankPlayer.cpp)
