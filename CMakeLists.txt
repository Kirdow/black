cmake_minimum_required(VERSION 3.20)

project(black)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(black "${SOURCE_FILES}")

target_precompile_headers(black PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")

