#include "ros/ros.h"
#include "gps_dr/final_msg.h"

void msgCallback(const gps_dr::final_msg::ConstPtr& msg)
{
    ROS_INFO("latitude: %lf, longitude: %lf",msg->final_latitude, msg->final_longitude);
}

int main(int argc, char **argv){

  ros::init(argc, argv, "final_msg_subscriber");

  ros::NodeHandle nh;

  ros::Subscriber ros_tutorial_sub = nh.subscribe("DR_msg", 100, msgCallback);

  ros::spin();

  return 0;
}
