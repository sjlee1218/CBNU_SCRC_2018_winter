cmake_minimum_required(VERSION 3.11)
project(serial_ubuntu)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(serial_ubuntu
        ceSerial.cpp
        ceSerial.h
		PVT_Parser.cpp
		PVT_Parser.h
        main_PVT.cpp)
