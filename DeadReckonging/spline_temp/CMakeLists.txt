cmake_minimum_required(VERSION 3.13)
project(spline_temp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(spline_temp
        CatmullRom.cpp
        CatmullRom.h
        Curve.cpp
        Curve.h
        Bezier.cpp
        Bezier.h
        BSpline.cpp
        BSpline.h
        main.cpp
        Vector.cpp
        Vector.h
        )
