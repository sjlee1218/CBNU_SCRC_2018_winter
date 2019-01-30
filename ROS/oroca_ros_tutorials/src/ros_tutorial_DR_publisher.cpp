#include "ros/ros.h"
#include "oroca_ros_tutorials/msgTutorial.h"
#include <math.h>

#define degree (M_PI/180.0)

double GPS_lat;
double GPS_lon;
double GPS_head;
double GPS_gSpeed;
ros::Time signal_time;

void DeadReck(double lon_lat[2]){

    ros::Duration elapsed = ros::Time::now()-signal_time;
    double elapsed_time = elapsed.toSec();
    std::cout<<"elapsed time: "<<elapsed_time<<std::endl;
    double moving_distance = GPS_gSpeed * elapsed_time; // cm

    double moved_lat_dist = moving_distance*sin(GPS_head*degree);
    double moved_lon_dist = moving_distance*cos(GPS_head*degree);

    double moved_lat_degree = moved_lat_dist/8888400;
    double moved_lon_degree = moved_lon_dist/11100000;

    lon_lat[0] = GPS_lon+moved_lon_degree; //0 for longitude
    lon_lat[1] = GPS_lat + moved_lat_degree;

}

void msgCallback(const oroca_ros_tutorials::msgTutorial::ConstPtr& msg)
{
    signal_time = ros::Time::now();
    GPS_lat = msg->latitude;
    GPS_lon = msg->longitude;
    GPS_head = msg->heading;
    GPS_gSpeed = msg->gSpeed;
    //ROS_INFO("Got GPS signal");
}

int main(int argc, char **argv){

  ros::init(argc, argv, "GPS_subscriber_DR_publisher");

  ros::NodeHandle nh;
  oroca_ros_tutorials::msgTutorial msg;

  ros::Subscriber ros_tutorial_sub = nh.subscribe("GPS_msg", 100, msgCallback);
  ros::Publisher DR_pub = nh.advertise<oroca_ros_tutorials::msgTutorial>("DR_msg", 100);


  double lon_lat[2];

  ros::Rate loop_rate(30);
//  ros::spin();

  while (ros::ok()) {
//      if (itr==4)
//          break;

      DeadReck(lon_lat);

      msg.longitude=lon_lat[0];
      msg.latitude=lon_lat[1];
      msg.flag=0; // 0 for DR

      DR_pub.publish(msg);

      loop_rate.sleep();
      ros::spinOnce();

  }// end of stdout while


  return 0;
}
