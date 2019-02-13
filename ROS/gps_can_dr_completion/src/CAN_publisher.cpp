#include "socketcan.h"
#include "ros/ros.h"

// include CAN_msg.msg
#include "gps_dr/CAN_msg.h"

int main(int argc, char** argv){

    ros::init(argc, argv, "CAN_Publisher");
    ros::NodeHandle nh;

    ros::Publisher can_pub = nh.advertise<gps_dr::CAN_msg>("CAN_msg", 100);

    socketCan temp_socket;
    CanMsg can_msg;
    int byte_num;

    temp_socket.init_can("vcan0");

    gps_dr::CAN_msg can_msg_to_send;

    can_msg_to_send.vehicleSpeed=0;
    can_msg_to_send.yaw_rate=0;

    while(1)
    {
        byte_num = temp_socket.read_msg(can_msg);
        if (can_msg.id == 544){ // yaw_rate
            can_msg_to_send.yaw_rate = (double)((uint16_t)(can_msg.data[5]) + ((uint16_t)(can_msg.data[6]&0x1f)<<8))*0.01 - 40.95;
            std::cout<<"Got yaw rate: "<<can_msg_to_send.yaw_rate<<std::endl;
        }
        if (can_msg.id == 1322){ //vehicle speed
            can_msg_to_send.vehicleSpeed = (double)((uint16_t)can_msg.data[0] & 0xff); //cluster vehicle speed
            std::cout<<"Got vehicleSpeed: "<<can_msg_to_send.vehicleSpeed<<std::endl;
        }

        can_pub.publish(can_msg_to_send);
        // should I use spinOnce(), or spin(), or something like that?
    }

    return 0;

}
