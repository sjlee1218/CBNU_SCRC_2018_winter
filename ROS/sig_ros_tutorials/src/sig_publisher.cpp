#include "ros/ros.h"
#include "sig_ros_tutorials/location.h"
#include "sig_ros_tutorials/ceSerial.h"
#include "sig_ros_tutorials/parser_vercpp.hpp"
#include <iostream>
#include <string>
#include <signal.h>

typedef unsigned char BYTE;

volatile int j=0;
void handler(int sig){
        j=1;
            return;
}

int main(int argc, char** argv){
    ros::init(argc, argv, "sig_ros_msg_publisher");
    ros::NodeHandle nh;

    ros::Publisher sig_pub = nh.advertise<sig_ros_tutorials::location>("sig_tutorial_msg",100);

    std::string portname = "/dev/ttyACM0";
    int baudrate = 9600;

    ublox_parser cUblox(portname, baudrate);
    ublox_parser::PARSING_TYPEDEF_UBX_M8P_PVT globalPVT;

    sig_ros_tutorials::location msg;
    double signal_lat=0;
    double signal_lon=0;
    double signal_head=0;
    double signal_speed=0;

    ros::Rate loop_rate(25); //Assume GPS sensor receive signal in 5hz

    while (cUblox.isInit && ros::ok()) {
        cUblox.run(); // once it ran, is valid always true? or true only when it got new data?

        if (cUblox.valid() == ublox_parser::PARSING_SUCCESS_) {
            cUblox.copyTo(&globalPVT);
            // because we should update our vehicle's location at least 5 times between two signals
            std::cout<<"Valid!!!!!!!!@$@!#$!@#%!@#%!@$#!!!!"<<std::endl;

            signal_lat=globalPVT.lat;
            signal_lon=globalPVT.lon;
            signal_head=globalPVT.headMot;
            signal_speed=globalPVT.gSpeed;

            msg.latitude= globalPVT.lat;
            msg.longitude=globalPVT.lon;
            msg.heading = globalPVT.headMot; // heading of motion
            msg.gSpeed=globalPVT.gSpeed;
            //also inefficient. it should be better with subtle change
            // by one variable, init

        }
        else{ // if cUblox.run() makes valid false, this code will executed
            // Dead Reckoning
            /*
            msg.latitude = DR(signal_lat,signal_lon,signal_head,signal_speed);
            msg.longitude = DR(signal_lat,signal_lon,signal_head,signal_speed);
            */
            std::cout<<"not Valid!"<<std::endl;

        }

        sig_pub.publish(msg);
        loop_rate.sleep();

    }// end of stdout while
}
