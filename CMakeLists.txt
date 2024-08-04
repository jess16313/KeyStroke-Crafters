cmake_minimum_required(VERSION 3.28)
project(KeyStroke_Crafters)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(KeyStroke_Crafters
        keystroke-crafters/HashTable.cpp
        keystroke-crafters/httplib.h
        keystroke-crafters/json.hpp
        keystroke-crafters/main.cpp
        keystroke-crafters/TypingGame.cpp
        keystroke-crafters/BST.cpp
        keystroke-crafters/TypingGame.h)

target_link_libraries(KeyStroke_Crafters
        ws2_32
)