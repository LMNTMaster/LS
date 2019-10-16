#pragma once

#define null nullptr
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning( disable : 4251)
/*windows.h 와 충돌나서 windows.h보다 먼저 선언되어야 함.*/
#include <winsock2.h>
#pragma comment(lib, "ws2_32")


#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <string.h>
#include <windows.h>
#include <list>
#include <tuple>
#include <atomic>
#include <process.h>


//#include <winsock.h>
//#pragma comment(lib, "wsock32.lib")

enum PACKET_STATE
{
	PACKET_INVALID_HEADER = 0x0000,
	PACKET_FUCNTION_CALL = 0x0045,
	PACKET_REQUEST_VALUE = 0x0066,
	PACKET_SYSTEM_MESSAGE = 0x0001,
	PACKET_STRING_MESSAGE = 0x0002,
};


#ifndef PORT
#define PORT 5965
#endif


#ifndef PACKET_SIZE
#define PACKET_SIZE 1024
#endif

#ifdef _WINDLL
#define	LMNT_DLL	__declspec(dllexport)
#else	
#define	LMNT_DLL	__declspec(dllimport)
#endif // LMNT_EXPORT
