#include <iostream>
#include <string>
#include "parser_vercpp.hpp"
#include <fstream>
#include <signal.h>
#include <string.h>

using namespace std;

volatile int j=0;

void handler(int sig){
	j=1;
	return;
}

int main(int argc, char *argv[])
{

    std::string portname = "/dev/ttyACM0";
    int baudrate = 9600;

    ublox_parser cUblox(portname, baudrate);
    ublox_parser::PARSING_TYPEDEF_UBX_M8P_PVT globalPVT;

	    // argc==1 -> stdout.
	    // argc==3, ./a.out 1 ~.txt
	    // argv[1]==flag, argv[2]==filename

    if (argc==1) { // stdout
        //signal(SIGINT, handler);
        while (cUblox.isInit) {
            cUblox.run();

            if (cUblox.valid() == ublox_parser::PARSING_SUCCESS_) {
                cUblox.copyTo(&globalPVT);
                cout << fixed;
                cout.precision(6);
                cout << globalPVT.lat << ", " << globalPVT.lon << ", " << globalPVT.height << endl<<endl;

            }
        }// end of stdout while
    }
    else if(argc==3 && (strcmp(argv[1],"1")==0)) { // save contents at argv[2]
        signal(SIGINT, handler);
        ofstream out(argv[2]);
        while (cUblox.isInit) {
            cUblox.run();

            if (j)
                break;

            if (cUblox.valid() == ublox_parser::PARSING_SUCCESS_) {
                cUblox.copyTo(&globalPVT);
                out << fixed;
                out.precision(6);
                out << globalPVT.lat << ", " << globalPVT.lon << ", " << globalPVT.height << endl<<endl;

            }
        }// end of file I/O out while
        out.close();
    }
    else{
        cout<<"Command error!"<<endl;
        cout<<"eg 1) ./hope_final -> stdout"<<endl;
        cout<<"eg 2) ./hope_final filename.txt -> save contents at argv[2]"<<endl;
        cout<<"if you want to save at file, argc must be 3 and argv[1] must be 1"<<endl;
    }

    return 0;
}
