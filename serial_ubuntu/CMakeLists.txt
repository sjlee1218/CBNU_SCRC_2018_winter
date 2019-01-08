project(serial_ubuntu)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(serial_ubuntu
        ceSerial.cpp
        ceSerial.h
        conserial.cpp)
