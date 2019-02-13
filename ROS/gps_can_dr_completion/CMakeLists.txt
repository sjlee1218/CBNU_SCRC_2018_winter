cmake_minimum_required(VERSION 2.8.3)
project(gps_dr)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  CAN_msg.msg
  prev_loc.msg
  final_msg.msg
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
  LIBRARIES gps_dr
  CATKIN_DEPENDS roscpp std_msgs
  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
  /home/avl/catkin_ws/src/gps_dr/include/gps_dr
)

add_executable(GPS_publisher src/GPS_publisher.cpp ${EXTSRCS} src/parser_vercpp.cpp ${EXTSRCS} src/ceSerial.cpp)
target_link_libraries(GPS_publisher ${catkin_LIBRARIES})
add_dependencies(GPS_publisher gps_dr_generate_messages_cpp)

add_executable(msg_subscriber src/msg_subscriber.cpp)
target_link_libraries(msg_subscriber ${catkin_LIBRARIES})
add_dependencies(msg_subscriber gps_dr_generate_messages_cpp)
 
add_executable(DR_publisher src/DR_publisher.cpp ${EXTSRCS} src/parser_vercpp.cpp ${EXTSRCS} src/ceSerial.cpp)
target_link_libraries(DR_publisher ${catkin_LIBRARIES})
add_dependencies(DR_publisher gps_dr_generate_messages_cpp)

add_executable(CAN_publisher src/CAN_publisher.cpp ${EXTSRCS} src/socketcan.cpp)
target_link_libraries(CAN_publisher ${catkin_LIBRARIES})
add_dependencies(CAN_publisher gps_dr_generate_messages_cpp)
