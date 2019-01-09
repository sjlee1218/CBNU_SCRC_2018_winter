// ~/NmpntLibrary/NtripClient/NtripClient.cpp
#ifdef _WIN32
	#include "stdafx.h"
#else
	#include <stdio.h>
	#include <string.h>
#endif
#include "../include/NmpntExtern.h"

NmpntClient			*__client = NULL;
char				__mountpoint[32];
unsigned int		__result = 0;
FILE				*__file = NULL;

// Callback Function //////////////////////////////////////////////////////////////////////////////
static void table(char *mount)
{
	//...
	printf("%s\n", mount);
}

static void ntrip(void *self, char *data, int size)
{
	printf("%s : ntrip : %d\n", __mountpoint, size);
	if (__file)
		fwrite(data, 1, size, __file);
}

static void state(void *self, int status)
{
	printf("%s : ", __mountpoint);
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

	// NTRIP Client 테스트
	__client = NmpntConstruct();
	sprintf(__mountpoint, "CCHJ_RTCM30");
	__file = fopen("CCHJ_RTCM30", "wb");

	getchar();
	NmpntStart(__client, NULL, __mountpoint, NTRIP_PURPOSE_TMP, &ntrip, &state);

	getchar();
	printf("status : %s / %d\n", __mountpoint, NmpntStatus(__client));

	//getchar();
	NmpntStop(__client);
	if (__file)
		fclose(__file);
	__file = NULL;

	getchar();
	// Mountpoints 변경
	sprintf(__mountpoint, "DAEZ_RTCM30");
	 __file = fopen("DAEZ_RTCM30", "wb");
	NmpntStart(__client, NULL, __mountpoint, NTRIP_PURPOSE_TRF, &ntrip, &state);

	getchar();
	printf("status : %s / %d\n", __mountpoint, NmpntStatus(__client));
			
	NmpntStop(__client);
	if (__file)
		fclose(__file);
	__file = NULL;

    getchar();
	// Mountpoints 변경
	sprintf(__mountpoint, "YNJU_RTCM32");
	__file = fopen("YNJU_RTCM32", "wb");

	NmpntStart(__client, NULL, __mountpoint, NTRIP_PURPOSE_TRF, &ntrip, &state);

	getchar();          
	printf("status : %s / %d\n", __mountpoint, NmpntStatus(__client));
				
	getchar();
	NmpntStop(__client);
    if (__file)                     
		fclose(__file);             
	__file = NULL;

	NmpntDestruct(__client);

#ifdef _WIN32
	WSACleanup();
#endif

	getchar();
	return 0;
}

