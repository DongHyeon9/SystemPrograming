#include "Client_WSAEventSelect.h"

bool Client_WSAEventSelect::Initialize()
{
	std::cout << "Initialize Client WSAEventSelect" << std::endl;

	if (!__super::Initialize()) return false;

	// 1. 소켓 생성
	socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket == INVALID_SOCKET)
	{
		std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	// 2. 논블로킹 소켓 변환
	u_long mode{};
	if (::ioctlsocket(socket, FIONBIO, &mode) == INVALID_SOCKET)
	{
		std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	// 3. 서버 연결
	// 서버정보 기입
	SOCKADDR_IN serverAddr{};
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	while (true)
	{
		if (::connect(socket, reinterpret_cast<const SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			// 이미 연결된 상태
			if (::WSAGetLastError() == WSAEISCONN)
				break;

			std::cout << "Connect Error Code : " << ::WSAGetLastError() << std::endl;
			return false;
		}
	}

	std::cout << "Connected To Server!" << std::endl;

	return true;
}

void Client_WSAEventSelect::Deinitialize()
{
	::closesocket(socket);
	__super::Deinitialize();
}

void Client_WSAEventSelect::Run()
{
	std::string sendBuffer{ "Hello Echo" };

	while (true)
	{
		if (::send(socket, sendBuffer.c_str(), static_cast<int32>(sendBuffer.size()), 0) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSAEWOULDBLOCK)
				continue;

			std::cout << "Send Error Code : " << ::WSAGetLastError() << std::endl;
			return;
		}

		std::cout << "Send Data! Data = " << sendBuffer << " / " << sendBuffer.size() << std::endl;

		while (true)
		{
			std::string recvBuffer(RECV_SIZE, 0);
			const int32 recvLen{ ::recv(socket, recvBuffer.data(), static_cast<int32>(recvBuffer.size()), 0) };
			if (recvLen == SOCKET_ERROR)
			{
				if (::WSAGetLastError() == WSAEWOULDBLOCK)
					continue;

				std::cout << "Recv Error Code : " << ::WSAGetLastError() << std::endl;
				return;
			}
			else if (recvLen == 0)
			{
				std::cout << "Disconnected" << std::endl;
			}

			std::cout << "Recv Data! Data = " << recvBuffer << " / " << recvBuffer.size() << std::endl;
			break;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
