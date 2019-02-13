#include <ceSerial.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace std;
namespace serial { class Serial; }

class ublox_parser {
public : 

	typedef enum {
		PARSING_SUCCESS_ = 0,
		PARSING_FAIL_ = -1
	} PARSING_STATUS;

	typedef struct {

		uint64_t now;

		double 		iTOW;
		uint16_t 	year;
		uint8_t 	month;
		uint8_t 	day;
		uint8_t 	hour;
		uint8_t 	min;
		uint8_t 	sec;
		uint8_t 	valid;
		double 		tAcc;
		double 		nano;
		uint8_t 	fixType;
		uint8_t 	flags;
		uint8_t 	flags2;
		uint8_t 	numSV;
		double 		lon;
		double 		lat;
		double 		height;
		double 		hMSL;
		double 		hAcc;
		double 		vAcc;
		double 		velN;
		double 		velE;
		double 		velD;
		double 		gSpeed;
		double 		headMot;
		double 		sAcc;
		double 		headAcc;
		double 		pDOP;
		double 		headVeh;

	} PARSING_TYPEDEF_UBX_M8P_PVT;

public : 
	void run();
	void start();
	PARSING_STATUS valid();
	void copyTo(PARSING_TYPEDEF_UBX_M8P_PVT *pDst);
	ublox_parser(const std::string port_name, int baudrate);
    ~ublox_parser();
	bool isInit;
    void set_valid_to_fail();

protected:
        PARSING_TYPEDEF_UBX_M8P_PVT mPVT;
	PARSING_STATUS mValid;

private:
	uint64_t little64(uint8_t *c);
	uint32_t little32(uint8_t *c);
	uint16_t little16(uint8_t *c);
    bool init; // added by me

	PARSING_STATUS c94_m8p_parser(const uint8_t ch);
	ce::ceSerial *serial_;

	//std::thread *pThread;
};

