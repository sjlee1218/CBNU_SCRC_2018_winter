#include "ros/ros.h"
#include "oroca_ros_tutorials/msgTutorial.h"
#include "ceSerial.h"
#include "parser_vercpp.hpp"
#include <iostream>
#include <string>
#include <signal.h>

typedef unsigned char BYTE;
/*
volatile int j=0;
void handler(int sig){
        j=1;
            return;
}
*/

int main(int argc, char** argv){
    ros::init(argc, argv, "ros_tutorial_msg_publisher");
    ros::NodeHandle nh;

    ros::Publisher ros_tutorial_pub = nh.advertise<oroca_ros_tutorials::msgTutorial>("GPS_msg", 100);

    std::string portname = "/dev/ttyUSB0";
    int baudrate = 9600;

    ublox_parser cUblox(portname, baudrate);
    ublox_parser::PARSING_TYPEDEF_UBX_M8P_PVT globalPVT;

    oroca_ros_tutorials::msgTutorial msg;

    ROS_INFO("GPS: 5hz, loop_rate: 10hz");

    ros::Rate loop_rate(20); //Assume GPS sensor receive signal in 5hz
    while (cUblox.isInit && ros::ok()) {
        cUblox.run(); // once it ran, is valid always true? or true only when it got new data?

        if (cUblox.valid() == ublox_parser::PARSING_SUCCESS_) {
            cUblox.copyTo(&globalPVT);

            // because we should update our vehicle's location at least 5 times between two signals
            //std::cout<<"Valid"<<std::endl;
            ROS_INFO("Publish GPS Message");

            msg.latitude= globalPVT.lat;
            msg.longitude=globalPVT.lon;
            msg.heading = globalPVT.headMot; // heading of motion
            msg.gSpeed=globalPVT.gSpeed;

            ros_tutorial_pub.publish(msg);
            loop_rate.sleep();
            cUblox.set_valid_to_fail();
            //ros::spinOnce();
        }
//        else{
//            cout<<"not valid"<<endl;
//        }

    }// end of stdout while
}
