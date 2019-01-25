#include "socketcan.h"
socketCan::socketCan() :
    read_bytes(0), write_bytes(0)
{
}
socketCan::~socketCan(){
}

int socketCan::init_can(const char *_dev_name){
    socketNum = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(socketNum <0)
        return -1;

    strcpy(ifr.ifr_name, _dev_name);
    ioctl(socketNum, SIOCGIFINDEX, &ifr);

    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    printf("%s at index %d\n", _dev_name, ifr.ifr_ifindex);

    if(bind(socketNum, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error in socket bind");
        return -2;
    }
}

int socketCan::read_msg(CanMsg &_msg){
    read_bytes = read(socketNum, &msg,sizeof(can_frame));
    if(read_bytes < 0)
        return -1;
    _msg.id = msg.can_id;
    _msg.dlc = msg.can_dlc;
    memcpy(_msg.data, msg.data, sizeof(__u8)*8);

    return read_bytes;
}

int socketCan::write_msg(CanMsg _msg){
    msg.can_id = _msg.id;
    msg.can_dlc = _msg.dlc;
    memcpy(msg.data, _msg.data, sizeof(__u8)*8);
    write_bytes = write(socketNum, &msg, sizeof(can_frame));
}

