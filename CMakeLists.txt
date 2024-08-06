cmake_minimum_required(VERSION 3.26.4)
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
        keystroke-crafters/TypingGame.h
        keystroke-crafters/main.cpp
        keystroke-crafters/main.cpp)

if (WIN32)
    message(STATUS "Configuring for Windows")
    target_link_libraries(KeyStroke_Crafters
            ws2_32
    )
elseif (APPLE)
    message(STATUS "Configuring for macOS")
    # Add macOS-specific settings if any
    # For example, you might need to link additional libraries or set specific compile options
elseif (UNIX)
    message(STATUS "Configuring for Unix/Linux")
    # Add Unix/Linux-specific settings if any
endif()