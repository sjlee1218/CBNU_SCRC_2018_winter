// File: conserial.cpp
// Description: Serial communication console program for Windows and Linux
// WebSite: http://cool-emerald.blogspot.sg/2017/05/serial-port-programming-in-c-with.html
// MIT License (https://opensource.org/licenses/MIT)
// Copyright (c) 2018 Yan Naing Aye

#include <stdio.h>
#include "ceSerial.h"
#include "PVT_Parser.h"
#include <iostream>
#include <fstream>
#include <signal.h>
using namespace std;
using namespace ce;


volatile int j=0;

void handler(int sig){
	j=1;
	return;
}

int main(int argc, char** argv)
{
	ceSerial com("/dev/ttyACM0",9600,8,'N',1); // Linux

	printf("Opening port %s.\n",com.GetPort().c_str());
	if (com.Open() == 0) {
		printf("OK.\n");
	}
	else {
		printf("Serial port connection Error.\n");
		return 0;
	}

	// argc==1 -> stdout.
	// argc==3, ./a.out 1 ~.txt
	// argv[1]==flag, argv[2]==filename


	if (argc==1) { // stdout

		// install SIGINT handler
		signal(SIGINT, handler);

		ublox_parser parser;
		ublox_parser::PARSING_TYPEDEF_UBX_M8P_PVT block;
		bool flag;
		while (1) {

			if (j)
				break;

			flag = parser.parse(com, &block);
			if (flag == false)
				continue;

			cout << "latitude: " << block.lat << endl;
			cout << "longitude: " << block.lon << endl<<endl;

		}
		return 0;
	}
	else if (argc==3 && (strcmp(argv[1],"1")==0)){ // save contents at argv[2]
		ublox_parser parser;
		ublox_parser::PARSING_TYPEDEF_UBX_M8P_PVT block;
		bool flag;

		ofstream out(argv[2]);

		if (!out.is_open()){
			printf("file error!");
			return 0;
		}

		signal(SIGINT, handler);
		while (1) {

			if (j)
				break;

			flag = parser.parse(com, &block);
			if (flag == false)
				continue;

			out << "latitude: " << block.lat << endl;
			out << "longitude: " << block.lon << endl<<endl;

		}
		out.close();
		return 0;
	}
	else{
		cout<<"Command error!"<<endl;
		cout<<"eg 1) ./a.out -> stdout"<<endl;
		cout<<"eg 2) ./a.out 1 filename.txt -> save contents at argv[2]"<<endl;
		cout<<"if you want to save at file, argc must be 3 and argv[1] must be 1"<<endl;
	}

	return 0;

}
