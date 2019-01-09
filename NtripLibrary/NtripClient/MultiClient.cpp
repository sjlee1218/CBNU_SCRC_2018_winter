// ~/NmpntLibrary/NtripClient/NtripClient.cpp
#ifdef _WIN32
	#include "stdafx.h"
#else
	#include <stdio.h>
	#include <string.h>
#endif
#include "MultiClient.h"

// Callback Function //////////////////////////////////////////////////////////////////////////////
static void table(char *mount)
{
	//...
	printf("%s\n", mount);
}

static void ntrip(void *self, char *data, int size)
{
	NtripClient 		*owner = (NtripClient *)self;

	printf("%s : ntrip : %d\n", owner->_mountpoint, size);
	if (owner->_result & NTRIP_FILESAVE)
	{
		//...
		if (owner->_file)
			fwrite(data, 1, size, owner->_file);
	}

	if (owner->_result & NTRIP_DECODE)
	{
		if (owner->_mountpoint[9] == '2') // RTCM 2.x
		{
			//...
		}
		else //if (owner->_mountpoint[9] == '3') // RTCM 3.x
		{
			//...
		}
	}

	if (owner->_result & NTRIP_NETWORK)
	{
		//...
	}

	if (owner->_result & NTRIP_SERIAL)
	{
		//...
	}
}

static void state(void *self, int status)
{
	printf("%s : ", ((NtripClient *)self)->_mountpoint);
	switch (status)
	{
	case NTRIP_ONAIR:
		printf("서비스를 시작하였습니다.\n");
		break;
	case NTRIP_OFFAIR:
		printf("서비스를 정지하였습니다.\n");
		break;
	case NTRIP_SOCKET_ERROR:
		printf("네트워크 오류입니다.\n");
		break;
	case NTRIP_CONNECT_ERROR:
		printf("네트워크 상태를 확인하십시요.\n");
		break;
	case NTRIP_TIMEOUT_ERROR:
		printf("네트워크 상태가 좋지 않습니다.\n");
		break;
	case NTRIP_CONNECT_CLOSE:
		printf("네트워크 연결이 끊겼습니다.\n");
		break;
	case NTRIP_MOUNT_ERROR:
		printf("지원하지 않는 마운트명입니다.\n");
		break;
	case NTRIP_USER_ERROR:
		printf("사용자정보가 틀립니다.\n");
		break;
	case NTRIP_VERSION_ERROR:
		printf("지원하지 않는 버전입니다.\n");
		break;
	case NTRIP_CASTER_ERROR:
		printf("정상적인 캐스터가 아닙니다.\n");
		break;
	default:
		printf("알 수 없는 오류입니다.\n");
		break;
	}
}

// class NtripClient //////////////////////////////////////////////////////////////////////////////
NtripClient::NtripClient()
{
	_client = NmpntConstruct();
	_result = 0;
	_file = NULL;
}

NtripClient::~NtripClient()
{
	stop();
	NmpntDestruct(_client);
	if (_file)
		fclose(_file);
}

void NtripClient::start(char *mountpoint, int purpose, unsigned int result)
{
	char			fname[256];

	strncpy(_mountpoint, mountpoint, 31);
	_result = result;

	stop();
	if (_result & NTRIP_FILESAVE)
	{
		sprintf(fname, "./%s.rtcm", _mountpoint);
		_file = fopen(fname, "wb");
	}

	if (_result & NTRIP_DECODE)
	{
		//...
	}

	if (_result & NTRIP_NETWORK)
	{
		//...
	}

	if (_result & NTRIP_SERIAL)
	{
		//...
	}

	NmpntStart(_client, this, _mountpoint, purpose, &ntrip, &state);
}

void NtripClient::stop()
{
	NmpntStop(_client);

	if (_file)
		fclose(_file);
	_file = NULL;
}

void NtripClient::status()
{
	printf("status : %s / %d\n", _mountpoint, NmpntStatus(_client));
}

// main ///////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	// socket 사용
#ifdef _WIN32
	WSADATA				wsa;

	WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

	// version 체크
	LibVersion			ver;

	ver = NmpntVersion();
	printf("library version : %04d.%02d.%02d\n", ver.major, ver.minor, ver.patch);

	// source table 체크
	printf("mountpoint count : %d\n", NmpntSourceTable(&table));

	// NTRIP Clients 테스트
	NtripClient			daez;
	NtripClient			dokd;
	NtripClient			cchn;

	getchar();
	daez.start((char *) "DAEZ_RTCM23", NTRIP_PURPOSE_RND, NTRIP_DECODE | NTRIP_NETWORK);
	dokd.start((char *) "DOKD_RTCM30", NTRIP_PURPOSE_CON, NTRIP_FILESAVE);
	cchn.start((char *) "CCHN_RTCM30", NTRIP_PURPOSE_EDU, NTRIP_SERIAL);

	getchar();
	daez.status();
	dokd.status();

	getchar();
	daez.stop();
	dokd.stop();

	getchar();
	// Mountpoints 변경
	daez.start((char *) "DAEZ_RTCM30", NTRIP_PURPOSE_ANF);
	dokd.start((char *) "DOKD_RTCM30", NTRIP_PURPOSE_EXP, NTRIP_FILESAVE | NTRIP_SERIAL);

	getchar();
	daez.stop();
	dokd.stop();
	cchn.stop();

#ifdef _WIN32
	WSACleanup();
#endif

	getchar();
	return 0;
}

