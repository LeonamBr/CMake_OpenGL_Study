CMAKE_MINIMUM_REQUIRED(VERSION 3.28)

SET(CMAKE_C_COMPILER c:/mingw64/bin/gcc.exe)
SET(CMAKE_CXX_COMPILER c:/mingw64/bin/g++.exe)

SET(CMAKE_CXX_STANDARD 20)

project(
    OpenGL
    VERSION 1.0.0
    LANGUAGES C CXX
)

add_executable(MainGL main.cpp)