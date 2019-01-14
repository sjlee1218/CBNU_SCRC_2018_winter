//
// Created by 이승준 on 2019-01-14.
//

#include "PVT_Parser.h"

bool ublox_parser::parse(ce::ceSerial com, ublox_parser::PARSING_TYPEDEF_UBX_M8P_PVT* PVT_struct){

    bool successFlag;
    BYTE rx;
    BYTE temp[96];

    rx = (BYTE) com.ReadChar(successFlag);
    if (rx != 0xB5)
        return false;
    rx = (BYTE) com.ReadChar(successFlag);
    if (rx != 0x62)
        return false;

    unsigned int check_A = 0;
    unsigned int check_B = 0;

    for (int i = 0; i < 96; i++) {
        temp[i] = (BYTE) com.ReadChar(successFlag);
        check_A = check_A + temp[i];
        check_A &= 0xff;
        check_B = check_B + check_A;
        check_B &= 0xff;
    }

    if (temp[0] != 0x01)
        return false;
    if (temp[1] != 0x07)
        return false;


    BYTE valid = temp[15];
    if ((valid & 0b111) != 0b111)
        return false;

    rx = (BYTE) com.ReadChar(successFlag);
    if (rx != check_A)
        return false;
    //printf("ck_A: %02x, my_caled_check_A: %02x\n", rx, check_A);
    rx = (BYTE) com.ReadChar(successFlag);
    if (rx != check_B)
        return false;

    PARSING_TYPEDEF_UBX_M8P_PVT toRet;
    int i=4;

    /* Parsing code */
    toRet.iTOW = little32(&temp[0 + i]) / 1000.0;						// second
    toRet.year = little16(&temp[4 + i]);																						// year
    toRet.month = temp[6 + i];																																					// month
    toRet.day = temp[7 + i];																																					// day
    toRet.hour = temp[8 + i];																																					// hour
    toRet.min = temp[9 + i];																																					// minutes
    toRet.sec = temp[10 + i];																																				// seconds
    toRet.valid = temp[11 + i];																																				// bit field
    toRet.tAcc = little32(&temp[12 + i]) / 1000000000.0;	// seconds
    toRet.nano = little32(&temp[16 + i]) / 1000000000.0;	// seconds
    toRet.fixType = temp[20 + i];																																				// unsigned number
    toRet.flags = temp[21 + i];																																				// bit field
    toRet.flags2 = temp[22 + i];																																				// bit field
    toRet.numSV = temp[23 + i];																																				// Unsigned number
    toRet.lon = little32(&temp[24 + i]) / 10000000.0;		// degree
    toRet.lat = little32(&temp[28 + i]) / 10000000.0;		// degree
    toRet.height = little32(&temp[32 + i]) / 1000.0;				// height above ellipsoid (meter)
    toRet.hMSL = little32(&temp[36 + i]) / 1000.0;				// height above mean sea level (meter)
    toRet.hAcc = little32(&temp[40 + i]) / 1000.0;				// Horizontal Accuracy Estimate (meter)
    toRet.vAcc = little32(&temp[44 + i]) / 1000.0;				// Vertical Accuracy Estimate (meter)
    toRet.velN = little32(&temp[48 + i]) / 1000.0;				// NED North Velocity (m/s)
    toRet.velE = little32(&temp[52 + i]) / 1000.0;				// NED East Velocity (m/s)
    toRet.velD = little32(&temp[56 + i]) / 1000.0;				// NED Down Velocity (m/s)
    toRet.gSpeed = little32(&temp[60 + i]) / 1000.0;				// Ground Speed (2D) (m/s)
    toRet.headMot = little32(&temp[64 + i]) / 100000.0;			// Heading of Motion (2D) (degree)
    toRet.sAcc = little32(&temp[68 + i]) / 1000.0;				// Speed Accuracy Estimation (m/s)
    toRet.headAcc = little32(&temp[72 + i]) / 100000.0;			// Heading Accuracy Estimate [Both Motion & Vehicle] (degree)
    toRet.pDOP = little16(&temp[76 + i]) / 100.0;																	// Position DOP
    toRet.headVeh = little32(&temp[84 + i]) / 100000.0;			// Heading of vehicle (2D) (degree)

    (*PVT_struct)=toRet;

    return true;
}


uint64_t ublox_parser::little64(uint8_t * c)
{
    uint8_t i = 0;
    uint64_t temp = 0x00;

    for (i = 7; i >= 0; i--)
    {
        temp |= c[i];

        if (i == 0) break;
        else temp = temp << 8;

    }

    return temp;
}

uint32_t ublox_parser::little32(uint8_t * c)
{
    uint8_t i = 0;
    uint32_t temp = 0x00;

    for (i = 3; i >= 0; i--)
    {
        temp |= c[i];

        if (i == 0) break;
        else temp = temp << 8;

    }

    return temp;
}

uint16_t ublox_parser::little16(uint8_t * c)
{
    uint8_t i = 0;
    uint16_t temp = 0x00;

    for (i = 1; i >= 0; i--)
    {
        temp |= c[i];

        if (i == 0) break;
        else temp = temp << 8;

    }

    return temp;
}
