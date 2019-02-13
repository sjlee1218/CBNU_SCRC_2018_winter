#include "ros/ros.h"
#include "gps_dr/prev_loc.h"
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

    ros::Publisher ros_tutorial_pub = nh.advertise<gps_dr::prev_loc>("GPS_msg", 100);

    std::string portname = "/dev/ttyUSB0";
    int baudrate = 9600;

    ublox_parser cUblox(portname, baudrate);
    ublox_parser::PARSING_TYPEDEF_UBX_M8P_PVT globalPVT;

    gps_dr::prev_loc msg;

    ROS_INFO("GPS: 5hz, loop_rate: 10hz");

    ros::Rate loop_rate(20); //Assume GPS sensor receive signal in 5hz
    while (cUblox.isInit && ros::ok()) {
        cUblox.run(); // once it ran, is valid always true? or true only when it got new data?

        if (cUblox.valid() == ublox_parser::PARSING_SUCCESS_) {
            cUblox.copyTo(&globalPVT);

            // because we should update our vehicle's location at least 5 times between two signals
            //std::cout<<"Valid"<<std::endl;
            ROS_INFO("Publish GPS Message");

            msg.pre_latitude= globalPVT.lat;
            msg.pre_longitude=globalPVT.lon;
            msg.pre_heading = globalPVT.headMot; // heading of motion
            msg.pre_gSpeed=globalPVT.gSpeed; // (m/s)

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
