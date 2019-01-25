#include <iostream>
#include <stdlib.h>
#include <ros/ros.h>
#include <thread>
#include "serial/serial.h"

//Vehicle Information Msg
#include <msgs/eps.h>
#include <msgs/brake.h>
#include <msgs/vcu.h>
#include <msgs/imu.h>

//#include <PCANBasic.h>
#include "socketcan.h"


const std::string topic_vcu = "vehicle_info/vcu";
const std::string topic_brake = "vehicle_info/brake";
const std::string topic_eps = "vehicle_info/eps";
const std::string topic_imu = "vehicle_info/imu";

void send_imu(void* nh);
bool running = true;

int main(int argc, char* argv[]){
    ros::init(argc, argv, "can_receive_node");
    ros::NodeHandle nh; //if you wirte this character "~" the the node name will be can_re~
    ros::Publisher eps_pub = nh.advertise<msgs::eps>(topic_eps,1);
    ros::Publisher vcu_pub = nh.advertise<msgs::vcu>(topic_vcu,1);
    ros::Publisher brake_pub = nh.advertise<msgs::brake>(topic_brake,1);

    std::thread th(send_imu, (void*)&nh);

    msgs::vcu vcu_info;
    msgs::eps eps_info;
    msgs::brake brake_info;
    msgs::imu imu_info;

//    TPCANMsg msg;
//    TPCANStatus status;
//    uint32_t count = 0;
//    status = CAN_Initialize(PCAN_USBBUS1, PCAN_BAUD_500K);
    socketCan scan;
    CanMsg sMsg;
    int stat;
    stat = scan.init_can("can0");

    while(ros::ok()){
        ros::spinOnce();
//        status = CAN_Read(PCAN_USBBUS1,&msg,NULL);
//        if(status !=PCAN_ERROR_OK){
////            char err[50];
////            CAN_GetErrorText(status, 0x09, err);
////            printf("%s\n",err);
//            continue;
//        }
        stat=scan.read_msg(sMsg);
//       if(msg.ID == 0x215){    //vcu
//           vcu_info.vcu_mode = msg.DATA[0] & 0x03;
//           vcu_info.motor_state = msg.DATA[4];
//           vcu_info.vehicle_speed = ((uint16_t)(msg.DATA[6]) | (uint16_t)(msg.DATA[7]<<8))/10;
//           vcu_pub.publish(vcu_info);
//       }
//       else if(msg.ID == 0x2B0){   //eps
//           eps_info.steer_angle = (((int16_t)(msg.DATA[1]<<8)|(int16_t)(msg.DATA[0])))*0.1;
//           eps_info.angle_speed = msg.DATA[2]*4;
//           eps_pub.publish(eps_info);
//       }
//       else if(msg.ID == 0x212){   //brake
//           brake_info.brake_mode = msg.DATA[0];
//           brake_info.brake_intensity = msg.DATA[1] * 0.5;
//           brake_pub.publish(brake_info);
//       }
        if(sMsg.id == 0x215){    //vcu
            vcu_info.vcu_mode = sMsg.data[0] & 0x03;
            vcu_info.motor_state = sMsg.data[4];
            vcu_info.vehicle_speed = ((uint16_t)(sMsg.data[6]) | (uint16_t)(sMsg.data[7]<<8))/10;
            vcu_pub.publish(vcu_info);
        }
        else if(sMsg.id== 0x2B0){   //eps
            eps_info.steer_angle = (((int16_t)(sMsg.data[1]<<8)|(int16_t)(sMsg.data[0])))*0.1;
            eps_info.angle_speed = sMsg.data[2]*4;
            eps_pub.publish(eps_info);
        }
        else if(sMsg.id == 0x212){   //brake
            brake_info.brake_mode = sMsg.data[0];
            brake_info.brake_intensity = sMsg.data[1] * 0.5;
            brake_pub.publish(brake_info);
        }
        else{
            continue;
        }

    }
    running = false;
    th.join();
    return 0;
}

//void send_imu(ros::NodeHandle* nh){
void send_imu(void* nh){
    ros::NodeHandle* n = (ros::NodeHandle*)nh;
    ros::Publisher imu_pub = n->advertise<msgs::imu>(topic_imu, 1);
    serial::Serial imu("/dev/ttyUSB0", 115200);
    msgs::imu imu_info;
    std::string data;
    uint8_t ch = 0x00;
    bool start = false;
    int idx = 0;
    std::vector<std::string> test;
    while(running){
        imu.read(&ch, 1);
//        test.push_back((char)ch);
        if((char)ch == '\n'){
            data.clear();
            start = true;
        }
        else if((char)ch == '\r' && start){
            std::cout<<data<<std::endl;
            char r[8],p[8],y[8];
            if(data.size()>=25){
                data.copy(r,8,0);
                data.copy(p,8,9);
                data.copy(y,8,18);
                imu_info.roll = atof(r);
                imu_info.pitch = atof(p);
                imu_info.yaw = atof(y);
                imu_pub.publish(imu_info);
            }
            data.clear();
            start = false;
        }
        else if(start){
            data.push_back((char)ch);
        }
        usleep(100);
    }
}
