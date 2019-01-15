cmake_minimum_required(VERSION 3.00)
project(serial_PVT_final)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(serial_PVT_final
        ceSerial.cpp
        ceSerial.h
        main.cpp
        parser_vercpp.cpp
        parser_vercpp.hpp)
