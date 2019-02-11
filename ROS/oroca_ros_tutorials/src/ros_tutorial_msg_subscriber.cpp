#include "ros/ros.h"
#include "oroca_ros_tutorials/msgTutorial.h"

void msgCallback(const oroca_ros_tutorials::msgTutorial::ConstPtr& msg)
{
    ROS_INFO("latitude: %lf, longitude: %lf",msg->latitude, msg->longitude);
}

int main(int argc, char **argv){

  ros::init(argc, argv, "final_msg_subscriber");

  ros::NodeHandle nh;

  ros::Subscriber ros_tutorial_sub = nh.subscribe("DR_msg", 100, msgCallback);

  ros::spin();

  return 0;
}
