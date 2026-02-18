#include "Client_IOCP.h"

static constexpr DWORD BUFFER_COUNT{ 1 };

bool Client_IOCP::Initialize()
{
	std::cout << "Initialize Client IOCP" << std::endl;

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

void Client_IOCP::Deinitialize()
{
	::closesocket(socket);
	__super::Deinitialize();
}

void Client_IOCP::Run()
{
	std::string sendBuffer{ "Hello Echo" };
	WSAOVERLAPPED overlapped{};
	overlapped.hEvent = ::WSACreateEvent();

	while (true)
	{
		WSABUF wsaSendBuf{};
		wsaSendBuf.buf = sendBuffer.data();
		wsaSendBuf.len = static_cast<uint32>(sendBuffer.size());

		DWORD length{};
		DWORD flags{};

		if (::WSASend(socket, &wsaSendBuf, BUFFER_COUNT, &length, flags, &overlapped, nullptr) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSA_IO_PENDING)
			{
				::WSAWaitForMultipleEvents(BUFFER_COUNT, &overlapped.hEvent, TRUE, WSA_INFINITE, FALSE);
				::WSAGetOverlappedResult(socket, &overlapped, &length, FALSE, &flags);
			}
			else
			{
				std::cout << "Send Error Code : " << ::WSAGetLastError() << std::endl;
				break;
			}
		}
		std::cout << "Send Data! Data = " << sendBuffer << " / " << sendBuffer.size() << std::endl;

		std::string recvBuffer(RECV_SIZE, 0);
		WSABUF wsaRecvBuf{};
		wsaRecvBuf.buf = recvBuffer.data();
		wsaRecvBuf.len = static_cast<uint32>(recvBuffer.size());

		if (::WSARecv(socket, &wsaRecvBuf, BUFFER_COUNT, &length, &flags, &overlapped, nullptr) == SOCKET_ERROR)
		{
			if (::WSAGetLastError() == WSA_IO_PENDING)
			{
				::WSAWaitForMultipleEvents(BUFFER_COUNT, &overlapped.hEvent, TRUE, WSA_INFINITE, FALSE);
				::WSAGetOverlappedResult(socket, &overlapped, &length, FALSE, &flags);
			}
			else
			{
				std::cout << "Recv Error Code : " << ::WSAGetLastError() << std::endl;
				break;
			}
		}

		std::cout << "Recv Data! Data = " << recvBuffer << " / " << recvBuffer.size() << std::endl;

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	::WSACloseEvent(overlapped.hEvent);
}
