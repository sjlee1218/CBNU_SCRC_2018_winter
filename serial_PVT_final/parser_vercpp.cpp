#include "parser_vercpp.hpp"


void ublox_parser::run()
{

	/*
	 * char ceSerial::ReadChar(bool& success)
{
	success=false;
	if (!IsOpened()) {return 0;	}
	success=read(fd, &rxchar, 1)==1;
	return rxchar;
}

	 */
	bool successFlag;
	uint8_t ch = 0x00;
	ch=(uint8_t)serial_->ReadChar(successFlag);
	if (successFlag==1){
		mValid = c94_m8p_parser(ch);
	}
}


void ublox_parser::start()
{
	if (isInit)
	{
        if (serial_->IsOpened())
		{
			//pThread = new std::thread(run);
		}
	}
}

ublox_parser::PARSING_STATUS ublox_parser::valid()
{
	return mValid;
}

void ublox_parser::copyTo(PARSING_TYPEDEF_UBX_M8P_PVT * pDst)
{
    (*pDst) = mPVT;
}

ublox_parser::ublox_parser(const std::string port_name, int baudrate) : 
	mValid(PARSING_FAIL_),
	isInit(false)
{

	//ceSerial com("/dev/ttyACM0",9600,8,'N',1); // Linux
    serial_ = new ce::ceSerial(port_name,baudrate, 8,'N',1);

	//mPVT = new PARSING_TYPEDEF_UBX_M8P_PVT;

    /*if (!serial_->IsOpened())
	{
		std::cout << "Connection Failed" << std::endl;
		return;
	}
*/

    printf("Opening port %s.\n",serial_->GetPort().c_str());
    if (serial_->Open() == 0) {
        printf("OK.\n");
    }
    else {
        printf("Serial port connection Error.\n");
        return;
    }


    isInit = true;
}

ublox_parser::~ublox_parser()
{
	
	//pThread->join();

    if (serial_->IsOpened())
	{
        serial_->Close();
	}


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

ublox_parser::PARSING_STATUS ublox_parser::c94_m8p_parser(const uint8_t ch)
{

	static volatile 	  uint8_t	flag		= 0x00;

	static const volatile uint8_t	_SYNC1		= 0xB5;
	static const volatile uint8_t	_SYNC2		= 0x62;

	static const volatile uint8_t	_CLASS_HPP	= 0x01;
	static const volatile uint8_t	_ID_HPP		= 0x14;
	static const volatile uint8_t	_CLASS_PVT	= 0x01;
	static const volatile uint8_t	_ID_PVT		= 0x07;

	static uint8_t 					payload[100] = { 0 };
	static volatile uint8_t 		count		= 0;

	if (flag == 0x00) flag = 0x01;

	/** @Parsing */
	switch (flag)
	{
	case 0x01:

		if (ch == _SYNC1) { flag += 0x01; }
		else { flag = 0x00; }
		break;

	case 0x02:

		if (ch == _SYNC2) { flag += 0x01; }
		else { flag = 0x00; count = 0; }
		break;

	case 0x03: 	/** @CompareCLASS */

		if (ch == _CLASS_PVT) { flag += 0x01; payload[count++] = ch; }
		else { flag = 0x00; count = 0; }
		break;

	case 0x04:		/** @CompareID */

		if (ch == _ID_PVT) { flag = 0x07; payload[count++] = ch; }
		else { flag = 0x00; count = 0; }
		break;

	case 0x07:		/** @PVT */

		payload[count++] = ch;

		if (count == 98)
		{
			count = 0;
			flag = 0x00;

			uint8_t i = 0x00;
			uint8_t CK_A = 0x00, CK_B = 0x00;

			for (i = 0; i < 96; i++)
			{
				CK_A = CK_A + payload[i];
				CK_B = CK_B + CK_A;
			}

			i = 4;
			if ((CK_A == payload[96]) && (CK_B == payload[97]))
			{
				/* To Parse the payload on format for use */
				PARSING_TYPEDEF_UBX_M8P_PVT toRet;

				/* Parsing code */
				toRet.iTOW = little32(&payload[0 + i]) / 1000.0;						// second
				toRet.year = little16(&payload[4 + i]);																						// year
				toRet.month = payload[6 + i];																																					// month
				toRet.day = payload[7 + i];																																					// day
				toRet.hour = payload[8 + i];																																					// hour
				toRet.min = payload[9 + i];																																					// minutes
				toRet.sec = payload[10 + i];																																				// seconds
				toRet.valid = payload[11 + i];																																				// bit field
				toRet.tAcc = little32(&payload[12 + i]) / 1000000000.0;	// seconds
				toRet.nano = little32(&payload[16 + i]) / 1000000000.0;	// seconds
				toRet.fixType = payload[20 + i];																																				// unsigned number
				toRet.flags = payload[21 + i];																																				// bit field
				toRet.flags2 = payload[22 + i];																																				// bit field
				toRet.numSV = payload[23 + i];																																				// Unsigned number
				toRet.lon = little32(&payload[24 + i]) / 10000000.0;		// degree
				toRet.lat = little32(&payload[28 + i]) / 10000000.0;		// degree
				toRet.height = little32(&payload[32 + i]) / 1000.0;				// height above ellipsoid (meter)
				toRet.hMSL = little32(&payload[36 + i]) / 1000.0;				// height above mean sea level (meter)
				toRet.hAcc = little32(&payload[40 + i]) / 1000.0;				// Horizontal Accuracy Estimate (meter)
				toRet.vAcc = little32(&payload[44 + i]) / 1000.0;				// Vertical Accuracy Estimate (meter)
				toRet.velN = little32(&payload[48 + i]) / 1000.0;				// NED North Velocity (m/s)
				toRet.velE = little32(&payload[52 + i]) / 1000.0;				// NED East Velocity (m/s)				
				toRet.velD = little32(&payload[56 + i]) / 1000.0;				// NED Down Velocity (m/s)
				toRet.gSpeed = little32(&payload[60 + i]) / 1000.0;				// Ground Speed (2D) (m/s)
				toRet.headMot = little32(&payload[64 + i]) / 100000.0;			// Heading of Motion (2D) (degree)
				toRet.sAcc = little32(&payload[68 + i]) / 1000.0;				// Speed Accuracy Estimation (m/s)
				toRet.headAcc = little32(&payload[72 + i]) / 100000.0;			// Heading Accuracy Estimate [Both Motion & Vehicle] (degree)
				toRet.pDOP = little16(&payload[76 + i]) / 100.0;																	// Position DOP
				toRet.headVeh = little32(&payload[84 + i]) / 100000.0;			// Heading of vehicle (2D) (degree)

                mPVT = toRet;

				return PARSING_SUCCESS_;

				break;


			} /** @EndOfIf if ( (CK_A == payload[96]) && (CK_B == payload[97]) ) */

		} /** @EndOfIf if ( count == 98 ) */

		break;

	default:
		break;

	} /** @EndOfSwitch switch(flag) */

	return PARSING_FAIL_;
	/** @EndOfParsing */

	//	__HAL_UART_CLEAR_PEFLAG(&huart3);
}
