cmake_minimum_required(VERSION 4.0)
project(MainProject)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(MainProject src/main.cpp)
