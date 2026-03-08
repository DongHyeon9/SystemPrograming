#pragma once
#include "Value.h"
#include "PreProcess.h"

// 내부적으로 OVERLAPPED를 사용하거나 상속받는 구조체는
// virtual 함수가 있으면 안된다
// -> 0번지 바이트가 가상함수 테이블 포인터로 덮어씌워져서 구조가 이상해져 이상하게 동작하거나 심하면 아예 크래시가 날 수 있다

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