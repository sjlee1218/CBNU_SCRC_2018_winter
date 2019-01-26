#include <iostream>
#include <PCANBasic.h>

#include <ros/ros.h>

//ioniq_module
//Vehicle Information Msg
#include "vehicle_msgs/steer.h"
#include "vehicle_msgs/misc.h"
#include "vehicle_msgs/speed.h"

const std::string topic_steer = "steer";
const std::string topic_speed = "speed";
const std::string topic_misc = "misc";

#define Can1 PCAN_USBBUS1

int main(int argc, char* argv[]){
    ros::init(argc, argv, "can_receiver");
    ros::NodeHandle nh("~"); //if you wirte this character "~" the the node name will be can_re~
    TPCANMsg msg;
    TPCANStatus stat;


    ros::Publisher steer_pub = nh.advertise<vehicle_msgs::steer>(topic_steer,1);
    ros::Publisher misc_pub = nh.advertise<vehicle_msgs::misc>(topic_misc,1);
    ros::Publisher speed_pub = nh.advertise<vehicle_msgs::speed>(topic_speed,1);


    stat = CAN_Initialize(Can1, PCAN_BAUD_500K);

    if(stat != PCAN_ERROR_OK){
        char error_message[50];
        CAN_GetErrorText(stat, 0x09, error_message);
        printf("%s\n",error_message);
        ROS_INFO("%s\n",error_message);
    }

    vehicle_msgs::steer steer_msg;
    vehicle_msgs::misc misc_msg;
    vehicle_msgs::speed speed_msg;

    while(ros::ok()){
        ros::spinOnce();
        stat = CAN_Read(Can1, &msg, NULL);
        if(stat !=PCAN_ERROR_OK){
            continue;
        }
        if(msg.ID == 0x710){
            steer_msg.apm_fd_en = msg.DATA[0] & 0x01;
            steer_msg.mo_fd_sate= (msg.DATA[0]>>1) & 0x3;
            steer_msg.steer_val = ((int16_t)(msg.DATA[2]) |(int16_t)(msg.DATA[3]<<8))*0.1;
            steer_msg.mod_fd_alvcnt= msg.DATA[7];
            steer_msg.errBit_EPS = msg.DATA[0]&0x20;
            steer_msg.errBit_PD =  msg.DATA[0]&0x10;
            steer_msg.steer_torq = 0;
            steer_msg.errBit_SAS = msg.DATA[0]&0x80 ;
            steer_msg.override = msg.DATA[1]&0x01;
            steer_pub.publish(steer_msg);
        }
        else if(msg.ID == 0x71F){
            misc_msg.switch_plus = msg.DATA[0] & 0x01;
            misc_msg.switch_minus = msg.DATA[0] & 0x02;
            misc_msg.switch_cancel = msg.DATA[0] & 0x04;

            misc_msg.switch_acc = msg.DATA[0] & 0x10;
            misc_msg.accel_pedal = msg.DATA[0] & 0x20;
            misc_msg.brake_pedal = msg.DATA[0] & 0x40;
            misc_msg.acc_enable = msg.DATA[0] & 0x80;
            misc_pub.publish(misc_msg);
        }
        else if(msg.ID ==0x711){
            speed_msg.asm_fd_en = msg.DATA[0] & 0x01;
            speed_msg.mo_fd_state= (msg.DATA[0]>>1) & 0x3;
            speed_msg.errbit_pd = msg.DATA[0] & 0x10;
            speed_msg.errbit_tcu = msg.DATA[0] &0x40;
            speed_msg.errbit_asm = msg.DATA[0] & 0x80;
            speed_msg.speed = msg.DATA[1];
            speed_msg.alv_cnt = msg.DATA[7];
            speed_pub.publish(speed_msg);
        }
            continue;
    }

    return 0;
}

