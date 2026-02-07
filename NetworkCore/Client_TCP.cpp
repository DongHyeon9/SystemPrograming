#include "Client_TCP.h"

bool Client_TCP::Initialize()
{
	std::cout << "Initialize Client TCP" << std::endl;

	if (!__super::Initialize()) return false;

	// 1. 소켓 생성
	clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	// 2. 서버 연결
	// 서버정보 기입
	SOCKADDR_IN serverAddr{};
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	if (::connect(clientSocket, reinterpret_cast<const SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cout << "Connect Error Code : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "Connected To Server!" << std::endl;

	return true;
}

void Client_TCP::Deinitialize()
{
	__super::Deinitialize();
}

void Client_TCP::Run()
{
	std::string sendBuffer{ "Hello Echo" };

	while (true)
	{
		if (::send(clientSocket, sendBuffer.c_str(), static_cast<int32>(sendBuffer.size()), 0) == SOCKET_ERROR)
		{
			std::cout << "Send Error Code : " << ::WSAGetLastError() << std::endl;
			return;
		}

		std::cout << "Send Data! Data = " << sendBuffer << " / " << sendBuffer.size() << std::endl;

		std::string recvBuffer(RECV_SIZE, 0);

		if (::recv(clientSocket, recvBuffer.data(), static_cast<int32>(recvBuffer.size()), 0) <= 0)
		{
			std::cout << "Recv Error Code : " << ::WSAGetLastError() << std::endl;
			return;
		}

		std::cout << "Recv Data! Data = " << recvBuffer << " / " << recvBuffer.size() << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
