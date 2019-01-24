#include "ros/ros.h"
#include <iostream>
#include "sig_ros_tutorials/location.h"

void msgCallback(const sig_ros_tutorials::location::ConstPtr& msg){
    ROS_INFO("lat: %f, lon: %f, head: %f, gSpeed: %f",msg->latitude, msg->longitude, msg->heading, msg->gSpeed);
}

int main(int argc, char** argv){
    ros::init(argc, argv, "msg_subscriber");
    ros::NodeHandle nh;

    ros::Subscriber sig_ros_subscriber = nh.subscribe("location_msg", 100, msgCallback);
    ros::spin();

    return 0;
}
