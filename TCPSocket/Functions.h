#pragma once
#include "CoreMinimal.h"

#if TCP_SOCKET
	#define TCP_API __declspec(dllexport)
#else
	#define TCP_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C"
#endif
{
	TCP_API SOCKET WINAPI CreateSocket();

	TCP_API int32 WINAPI Connect(SOCKET Socket, const SOCKADDR_IN& ServerAddress);
	TCP_API int32 WINAPI Bind(SOCKET Socket, const SOCKADDR_IN& ServerAddress);
	TCP_API int32 WINAPI Listen(SOCKET Socket);
	TCP_API SOCKET WINAPI Accept(SOCKET ServerSocket, SOCKADDR_IN& ClientAddr, int32& AddrLen);

	TCP_API int32 WINAPI Send(SOCKET Socket, const std::string& SendData);
	TCP_API int32 WINAPI Recv(SOCKET Socket, std::string& RecvBuffer, size_t BufferSize);
}