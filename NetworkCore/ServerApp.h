#pragma once
#include "CoreMinimal.h"

#define MODULE_NAME _T("TCPSocket")

class NETWORK_API ServerApp : public Singleton<ServerApp>
{
public:
	bool Initialize();
	void Deinitialize();

	bool Send(const std::string& SendData);
	bool Recv(std::string& Recv, size_t BufferSize);

private:
	bool InitializeModule();
	bool CreateSocket();
	bool BindSocket();
	bool Listen();
	bool AcceptClient();

private:
	HMODULE module{};

private:
	SOCKADDR_IN serverAddr{};
	SOCKET socket{};

	// 후에 Session으로 묶일예정
	SOCKET clientSocket{};
	SOCKADDR_IN clientAddr{};
	int32 addrLen{ sizeof(clientAddr) };
};