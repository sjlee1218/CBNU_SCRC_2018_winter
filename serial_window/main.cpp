#include <stdio.h>
#include "serialcomm.h"
#include <iostream>

//int BYTE_To_Int(BYTE temp);

#define M_PI 3.14159265358979323846

unsigned long get_reverse(BYTE temp[76], int offset, int lenth);
using namespace std;
int main()
{
	CSerialComm serialComm;
	// �ؿ� ���� �� ���� ���� �߰��Ѱ�
	BYTE rx;
	//BYTE toRead[3]; // [0] == Class, [1]== ID [2] == Length
	//BYTE payload[72];
	BYTE temp[76];
	unsigned long longitude;
	double lon;

	const float _deg2rad = M_PI / 180.0;

	//char buf[40];
	//char* str_arr[12];

	// STEP 1. SerialPort Connect
	if (!serialComm.connect("COM5"))
	{
		printf("connect faliled");
		return -1;
	}
	else
		printf("connect successed\n");

	while (1) {
		/*
		if (serialComm.serial.CSerialPort::ReadString(buf)) {
			int str_cnt = 0;
			char *tok = strtok(buf, ",");
			if (strcmp(tok, "$GNGGA") == 0) {
				while (tok != nullptr) {
					//printf("%d ,%s\n", str_cnt,tok);
					str_arr[str_cnt++] = tok;
					tok = strtok(nullptr, ",");
				}
				//std::cout << "lat: " << str_arr[2] << "," << str_arr[3] << std::endl;
				//std::cout << "lon: " << str_arr[4] << "," << str_arr[5] << std::endl;

				float lat = atof(str_arr[2]);
				float lon = atof(str_arr[4]);
				printf("lat: %f\n", lat/100);
				printf("lon: %f\n", lon/100);

				buf[0] = { 0, };
				fflush(stdout);
			}

		}
		else
			std::cout << "could not read buffer" << std::endl;
		*/


		if (serialComm.serial.CSerialPort::ReadByte(rx)) {
			if (rx == 0xB5) {
				serialComm.serial.CSerialPort::ReadByte(rx);
				if (rx == 0x62) {


					unsigned int check_A = 0;
					unsigned int check_B = 0;

					for (int i = 0; i < 96; i++) {
						serialComm.serial.CSerialPort::ReadByte(temp[i]);
						check_A = check_A + temp[i];
						check_A &= 0xff;
						check_B = check_B + check_A;
						check_B &= 0xff;
					}

					if (temp[0] != 0x01)
						continue;
					if (temp[1] != 0x07)
						continue;


					BYTE valid = temp[15];
					if (valid & 0b1111 != 0b1111)
						continue;

					serialComm.serial.CSerialPort::ReadByte(rx);
					if (rx != check_A)
						continue;
					//printf("ck_A: %02x, my_caled_check_A: %02x\n", rx, check_A);
					serialComm.serial.CSerialPort::ReadByte(rx);
					if (rx != check_B)
						continue;
					//printf("ck_B: %02x, my_caled_check_A: %02x\n", rx, check_B);

					// https://www.u-blox.com/sites/default/files/products/documents/u-blox8-M8_ReceiverDescrProtSpec_%28UBX-13003221%29_Public.pdf
					// according to above link (p. 328, 33.17.14 UBX-NAV-PVT (0x01 0x07)) for payload offset and length, use get_reverse function to get info such as longitude, latitude
					cout << fixed;
					cout.precision(6);
					std::cout << "longitude: " << (double)get_reverse(temp, 24, 4)*1e-7 << std::endl;
					std::cout << "latitude: " << (double)get_reverse(temp, 28, 4)*1e-7 << std::endl;
					//std::cout << "headVeh: " << (double)get_reverse(temp, 84, 4)*1e-5 << std::endl;
					std::cout << "hMSL(m): " << (double)get_reverse(temp, 36, 4)/1000<< std::endl;


					//printf("before reverse: %x %x %x %x\n", temp[28], temp[29], temp[30], temp[31]);

					//longitude = temp[31] << 24 | temp[30] << 16 | temp[29] << 8 | temp[28];

					//printf("longitude:just reverse %x\n",longitude);
					//lon =(double) longitude*1e-7;
					//printf("longitude:scale 1e-7 %f\n", lon);
					//lon = (double)lon * _deg2rad;
					//printf("longitude:scale 1e-7,rad %f\n",lon);


					/*
					for (int i = 0; i < 3; i++) {
						serialComm.serial.CSerialPort::ReadByte(toRead[i]);
					}
					if (toRead[0] != 0x01)
						continue;
					if (toRead[1] != 0x07)
						continue;

					for (int i = 0; i < 92; i++) {
						serialComm.serial.CSerialPort::ReadByte(payload[i]);
					}
					//payload[11] �� valid Byte
					BYTE valid = payload[11];

					if (valid & 0b1111 != 0b1111) // valid check 1Byte = 8bit, ____ ____ least 4 bits are valid bit
						continue;
						*/

					//std::cout << "lon: " << (int)payload[24] <<" "<< (int)payload[25]<<" "<< (int)payload[26] <<" "<< (int)payload[27] << std::endl;

					//some func
					//for (int i = 0; i < 3; i++) {
					//	serialComm.serial.CSerialPort::ReadByte(toRead[i]);
					//}
					//for (int i = 0; i < 92; i++) {
					//	serialComm.serial.CSerialPort::ReadByte(payload[i]);
					//}

					//Read_payload(toRead, payload);
					//bool CSerialPort::ReadByte(BYTE* &resp, UINT size)  

				//	if (serialComm.serial.CSerialPort::ReadByte(toRead))
				//		printf("read class, ID, Length\n");
				//	else
				//		printf("can't Read class, ID, Length\n");

				//	if (serialComm.serial.CSerialPort::ReadByte(payload, 92*8)) {

				//		std::cout << "longitude: " << payload[24] << payload[25] << payload[26] << payload[27] << std::endl;
				//		std::cout << "latitude: " << payload[28] << payload[29] << payload[30] << payload[31] << std::endl;
				//	}
				//	else
				//		printf("Could not Read payload\n");
				}
			}
		}
		//else
			//printf("Can not read buffer\n");
	}

	// STEP 2. Send Command
	//if(!serialComm.sendCommand('A'))
	//{	
	//	printf("send command failed\n");
	//}
	//else
	//	printf("send Command success\n");


	// STEP 3. SerialPort disConnect
	serialComm.disconnect();

	printf("\nend connect\n");
		return 0;

}

unsigned long get_reverse(BYTE temp[76], int offset, int length) {
	unsigned long ref = 0;

	//longitude = temp[31] << 24 | temp[30] << 16 | temp[29] << 8 | temp[28]

	for (int i = 0; i < length; i++) {
		ref |= temp[offset + 4 + i] << 8*i;
	}
	return ref;
}

/*
int BYTE_To_Int(BYTE temp) {
	//BYTE == unsigned char

	int ret = (int)(temp);

	return ret;
}
*/