cmake_minimum_required(VERSION 3.13)
project(Final)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Final
        ceSerial.cpp
        ceSerial.h
        conserial.cpp)
