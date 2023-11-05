cmake_minimum_required(VERSION 3.20)

project(black)

set(CMAKE_CXX_STANDARD 17)

add_executable(black src/main.cpp src/lexer.cpp src/compiler.cpp)

