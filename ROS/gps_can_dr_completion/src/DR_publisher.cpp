#include "ros/ros.h"
#include "gps_dr/prev_loc.h"
#include "gps_dr/CAN_msg.h"
#include "gps_dr/final_msg.h"
#include <math.h>

#define degree (M_PI/180.0)

double pre_lat;
double pre_lon;
double pre_head;

double yaw_rate;
double vehicleSpeed;

ros::Time pre_time;

void DeadReck(double lon_lat[2]){

    ros::Duration elapsed = ros::Time::now()-pre_time;
    pre_time = ros::Time::now();
    double elapsed_time = elapsed.toSec();
    std::cout<<"elapsed time: "<<elapsed_time<<std::endl;
    double moving_distance = vehicleSpeed * elapsed_time; // m/s * s = m

    pre_head += yaw_rate * elapsed_time;

    double moved_lat_dist = moving_distance*cos(pre_head*degree);
    double moved_lon_dist = moving_distance*sin(pre_head*degree);

    double moved_lat_degree = moved_lat_dist/11100000;
    double moved_lon_degree = moved_lon_dist/8888400;

    pre_lon = pre_lon+moved_lon_degree;
    pre_lat = pre_lat + moved_lat_degree;

    lon_lat[0] = pre_lon;
    lon_lat[1] = pre_lat;

}

void msgCallback(const gps_dr::prev_loc::ConstPtr& msg)
{
    pre_time = ros::Time::now();
    pre_lat = msg->pre_latitude;
    pre_lon = msg->pre_longitude;
    pre_head = msg->pre_heading;
    vehicleSpeed = msg->pre_gSpeed; // unit of gSpeed is m/s
    ROS_INFO("GPS signal,heading: %f, gSpeed: %f",pre_head, vehicleSpeed);
}

void canCallback(const gps_dr::CAN_msg::ConstPtr& msg){
    yaw_rate = msg->yaw_rate;
    vehicleSpeed = (msg->vehicleSpeed/3600)*1000 ; // can not test. I assumed that unit of cluster's speed is kph.
    // 1km/h == 1000m/3600s == 0.27777777777m/s
}

int main(int argc, char **argv){

  ros::init(argc, argv, "GPS_subscriber_DR_publisher");

  ros::NodeHandle nh;
  gps_dr::final_msg final_msg;

  ros::Subscriber ros_tutorial_sub = nh.subscribe("GPS_msg", 100, msgCallback);
  ros::Subscriber can_sub = nh.subscribe("CAN_msg", 100, canCallback);
  ros::Publisher DR_pub = nh.advertise<gps_dr::final_msg>("DR_msg", 100);


  double lon_lat[2];

  ros::Rate loop_rate(30);
//  ros::spin();

  while (ros::ok()) {

      DeadReck(lon_lat);

      final_msg.final_longitude  =lon_lat[0];
      final_msg.final_latitude=lon_lat[1];

      DR_pub.publish(final_msg);

      loop_rate.sleep();
      ros::spinOnce();

  }// end of stdout while


  return 0;
}
