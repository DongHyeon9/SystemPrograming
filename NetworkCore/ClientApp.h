#pragma once
#include "CoreMinimal.h"

#define MODULE_NAME _T("TCPSocket")

class NETWORK_API ClientApp : public Singleton<ClientApp>
{
public:
	bool Initialize();
	void Deinitialize();

	bool Send(const std::string& SendData);
	bool Recv(std::string& Recv, size_t BufferSize);

private:
	bool InitializeModule();
	bool CreateSocket();
	bool ConnectToServer();

private:
	HMODULE module{};

private:
	SOCKET clientSocket{};
	SOCKADDR_IN serverAddr{};
};