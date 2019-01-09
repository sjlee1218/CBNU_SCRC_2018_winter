// ~/NmpntLibrary/NtripClient/NtripClient.h
#pragma once
#include "../include/NmpntExtern.h"

// Bit Masking Flag for NTRIP Result
#define NTRIP_FILESAVE		0x00000001
#define NTRIP_DECODE		0x00000002
#define NTRIP_NETWORK 		0x00000004
#define NTRIP_SERIAL		0x00000008

class NtripClient
{
public:
	NtripClient();
	virtual ~NtripClient();

	NmpntClient			*_client;
	char				_mountpoint[32];
	unsigned int		_result;
	FILE				*_file;

	void start(char *mountpoint, int purpose = NTRIP_PURPOSE_ETC, unsigned int result = 0);
	void stop();
	void status();
};

