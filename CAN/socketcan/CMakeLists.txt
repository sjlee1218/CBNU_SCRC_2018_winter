cmake_minimum_required(VERSION 2.8.3)
project(socketcan)

add_compile_options(-std=c++11)

include_directories(
	include
)

add_library(socketcan src/socketcan.cpp)


#install(TARGETS socketcan
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION ./
#)
