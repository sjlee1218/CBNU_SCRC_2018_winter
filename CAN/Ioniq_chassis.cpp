#include <iostream>
#include <PCANBasic.h>
#include <ros/ros.h>

//Vehicle Information Msg
#include "vehicle_msgs/abs.h"
#include "vehicle_msgs/esc.h"

const std::string topic_abs= "abs";
const std::string topic_esc = "esc";

#define Can PCAN_USBBUS2
const double scale_factor = 0.03125;

int main(int argc, char* argv[]){
    ros::init(argc, argv, "chassi_receiver");
    ros::NodeHandle nh("~"); //if you wirte this character "~" the the node name will be can_re~
    TPCANMsg msg;
    TPCANStatus stat;


    ros::Publisher abs_pub = nh.advertise<vehicle_msgs::abs>(topic_abs,1);
    ros::Publisher esc_pub = nh.advertise<vehicle_msgs::esc>(topic_esc,1);


    stat = CAN_Initialize(Can, PCAN_BAUD_500K);

    if(stat != PCAN_ERROR_OK){
        char error_message[50];
        CAN_GetErrorText(stat, 0x09, error_message);
        printf("%s\n",error_message);
        ROS_INFO("%s\n",error_message);
    }

    vehicle_msgs::abs abs_msg;
    vehicle_msgs::esc esc_msg;

    while(ros::ok()){
        ros::spinOnce();
        stat = CAN_Read(Can, &msg, NULL);
        if(stat !=PCAN_ERROR_OK){
            continue;
        }
        if(msg.ID == 0x386){
            abs_msg.whl_spd_fl = (double)((uint16_t)(msg.DATA[0]) + ((uint16_t)(msg.DATA[1]&0x3f)<<8))*scale_factor;
            abs_msg.whl_spd_fr = (double)((uint16_t)(msg.DATA[2]) + ((uint16_t)(msg.DATA[3]&0x3f)<<8))*scale_factor;
            abs_msg.whl_spd_rl = (double)((uint16_t)(msg.DATA[4]) + ((uint16_t)(msg.DATA[5]&0x3f)<<8))*scale_factor;
            abs_msg.whl_spd_rr = (double)((uint16_t)(msg.DATA[6]) + ((uint16_t)(msg.DATA[7]&0x3f)<<8))*scale_factor;
            abs_pub.publish(abs_msg);
        }
        else if(msg.ID == 544){
            esc_msg.lat_accel = (double)((uint16_t)(msg.DATA[0]) + ((uint16_t)(msg.DATA[1]&0b111)<<8))*0.01 - 10.23;
            esc_msg.long_accel = (double)((uint16_t)((msg.DATA[1]&0xe0)>>5) + ((uint16_t)(msg.DATA[2])<<3))*0.01 - 10.23;
            esc_msg.yaw_rate = (double)((uint16_t)(msg.DATA[5]) + ((uint16_t)(msg.DATA[6]&0x1f)<<8))*0.01 - 40.95;
            esc_pub.publish(esc_msg);

        }
        continue;
    }

    return 0;
}

