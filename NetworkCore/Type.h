#pragma once
#include "Header.h"

using BYTE = unsigned char;

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using CREATE_SOCKET = SOCKET(WINAPI*)();
using CONNECT = int32(WINAPI*)(SOCKET, const SOCKADDR_IN&);
using BIND = int32(WINAPI*)(SOCKET, const SOCKADDR_IN&);
using LISTEN = int32(WINAPI*)(SOCKET);
using ACCEPT = SOCKET(WINAPI*)(SOCKET, SOCKADDR_IN&, int32&);
using SEND = int32(WINAPI*)(SOCKET, const std::string&);
using RECV = int32(WINAPI*)(SOCKET, std::string&, size_t);
