#include <iostream>
#include <string>
#include "parser_vercpp.hpp"

using namespace std;

int main(int argc, char *argv[])
{

    /*
    f (argc != 3)
    {
        std::cerr << "Argument Failed : parser_node [port] [baudrate]" << std::endl;
        return -1;
    }
     */
    std::string portname = "/dev/ttyACM0";
    std::string baud_str = "9600";
    int baudrate = atoi(baud_str.c_str());

    // ceSerial com("/dev/ttyACM0",9600,8,'N',1); // Linux

    ublox_parser cUblox(portname, baudrate);
    ublox_parser::PARSING_TYPEDEF_UBX_M8P_PVT globalPVT;


    while (cUblox.isInit)
    {
        cUblox.run();

        if (cUblox.valid() == ublox_parser::PARSING_SUCCESS_)
        {
            cUblox.copyTo(&globalPVT);
            std::cout<< globalPVT.lat << ", " << globalPVT.lon << ", " << globalPVT.height << std::endl;

        }
    }


    return 1;
}
