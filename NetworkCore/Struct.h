#pragma once
#include "Value.h"
#include "PreProcess.h"

struct NETWORK_API Session
{
	SOCKET socket{ INVALID_SOCKET };
	SOCKADDR_IN addr{};
	std::string buffer{ std::string(RECV_SIZE,0) };
	int32 recvBytes{};
	int32 sendBytes{};
};

struct NETWORK_API Session_Overlapped
{
	WSAOVERLAPPED overlapped{};
	SOCKET socket{ INVALID_SOCKET };
	SOCKADDR_IN addr{};
	std::string buffer{ std::string(RECV_SIZE,0) };
	int32 recvBytes{};
	int32 sendBytes{};
};

struct OverlappedEx
{
	WSAOVERLAPPED overlapped{};
	WSABUF wsaBuf{};
	uint8 type{};
};