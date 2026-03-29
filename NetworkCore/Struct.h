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

struct NETWORK_API Session_Asio : public std::enable_shared_from_this<Session_Asio>
{
	Session_Asio(boost::asio::ip::tcp::socket Socket) :socket(std::move(Socket)) {}
	void Start() { DoRecv(); }
	void DoRecv()
	{
		socket.async_receive(boost::asio::buffer(buffer.data(), BUFFER_SIZE),
			[this](boost::system::error_code ErrCode, std::size_t RecvSize) {
				if (ErrCode)
				{
					std::cout << "Recv Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
					return;
				}
				std::cout << "Recv [" << this << "] : " << buffer.data() << std::endl;
				DoSend(RecvSize);
			});
	}

	void DoSend(std::size_t Length)
	{
		socket.async_send(boost::asio::buffer(buffer.data(), Length),
			[this](boost::system::error_code ErrCode, std::size_t SendSize) {
				if (ErrCode)
				{
					std::cout << "Send Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
					return;
				}
				std::string sendData{ buffer.data(), SendSize };
				std::cout << "Send [" << this << "] : " << sendData << std::endl;
				DoRecv();
			});
	}

	boost::asio::ip::tcp::socket socket;
	std::array<char, BUFFER_SIZE> buffer{};
};