#ifndef SOCKETCAN_H
#define SOCKETCAN_H

//For Socket Can
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>
//End

struct CanMsg{
    __u32 id;
    __u8 dlc;
    __u8 data[8];
};

class socketCan
{
private:
    int read_bytes;
    int write_bytes;
    int socketNum;
    struct sockaddr_can addr;   //socket can adderss
    struct can_frame msg;
    struct ifreq ifr;           //Interface require struct
public:
    socketCan();
    ~socketCan();
    int init_can(const char* _dev_name);
    int read_msg(CanMsg& _msg);
    //int write_msg(CanMsg msg);
};

#endif // SOCKETCAN_H
