#pragma once
#include "CoreMinimal.h"

#if UDP_SOCKET
#define UDP_API __declspec(dllexport)
#else
#define UDP_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
#endif
{
	UDP_API SOCKET WINAPI CreateSocket();

	UDP_API int32 WINAPI Connect(SOCKET Socket, const SOCKADDR_IN& ServerAddress);
	UDP_API int32 WINAPI Bind(SOCKET Socket, const SOCKADDR_IN& ServerAddress);
	UDP_API SOCKET WINAPI Accept(SOCKET ServerSocket, SOCKADDR_IN& ClientAddr, int32& AddrLen);

	UDP_API int32 WINAPI Send(SOCKET Socket, const std::string& SendData);
	UDP_API int32 WINAPI Recv(SOCKET Socket, std::string& RecvBuffer, size_t BufferSize);
}