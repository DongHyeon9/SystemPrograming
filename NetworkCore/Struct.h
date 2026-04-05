#pragma once
#include "Value.h"

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

template<E_PROTOCOL_TYPE PROTOCOL>
struct NETWORK_API Session_Asio;

template<>
struct NETWORK_API Session_Asio<E_PROTOCOL_TYPE::TCP> : public std::enable_shared_from_this<Session_Asio<E_PROTOCOL_TYPE::TCP>>
{
	Session_Asio(boost::asio::ip::tcp::socket Socket) :socket(std::move(Socket)) {}
	void Start() { Recv(); }
	void Send(std::string SendData, std::size_t SendSize) 
	{
		::memcpy(buffer.data(), SendData.data(), SendSize);
		socket.async_send(
			boost::asio::buffer(buffer.data(), SendSize),
			[self = shared_from_this()](boost::system::error_code ErrCode, std::size_t SendSize)
			{
				self->Send_Internal(SendSize);
			});
	}

private:
	void Recv()
	{
		socket.async_receive(
			boost::asio::buffer(buffer.data(), BUFFER_SIZE),
			[self = shared_from_this()](boost::system::error_code ErrCode, std::size_t RecvSize) 
			{
				if (ErrCode)
				{
					std::cout << "Recv Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
					return;
				}
				std::cout << "Recv [" << self.get() << "] : " << self->buffer.data() << std::endl;
				std::string sendData{ self->buffer.data(), RecvSize };
				self->Send(sendData, RecvSize);
			});
	}

	void Send_Internal(std::size_t Length)
	{
		socket.async_send(boost::asio::buffer(buffer.data(), Length),
			[self = shared_from_this()](boost::system::error_code ErrCode, std::size_t SendSize) 
			{
				if (ErrCode)
				{
					std::cout << "Send Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
					return;
				}
				std::string sendData{ self->buffer.data(), SendSize };
				std::cout << "Send [" << self.get() << "] : " << sendData << std::endl;
				self->Recv();
			});
	}

private:
	boost::asio::ip::tcp::socket socket;
	std::array<char, BUFFER_SIZE> buffer{};
};

template<>
struct NETWORK_API Session_Asio<E_PROTOCOL_TYPE::UDP> : public std::enable_shared_from_this<Session_Asio<E_PROTOCOL_TYPE::UDP>>
{
	Session_Asio(boost::asio::ip::udp::socket Socket) :socket(std::move(Socket)) {}
	void Start() { RecvFrom(); }
	void SetRemoteEndpoint(boost::asio::ip::udp::endpoint RemoteEndpoint) { remoteEndpoint = std::move(RemoteEndpoint); }
	void SendTo(std::string SendData, std::size_t SendSize)
	{
		::memcpy(buffer.data(), SendData.data(), SendSize);
		socket.async_send_to(boost::asio::buffer(buffer.data(), SendSize), remoteEndpoint,
			[self = shared_from_this()](boost::system::error_code ErrCode, std::size_t SendSize)
			{
				self->Send_Internal(SendSize);
			});
	}

private:
	void RecvFrom()
	{
		socket.async_receive_from(
			boost::asio::buffer(buffer.data(), BUFFER_SIZE), remoteEndpoint,
			[self = shared_from_this()](boost::system::error_code ErrCode, std::size_t RecvSize) 
			{
				if (ErrCode)
				{
					std::cout << "Recv Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
					return;
				}
				std::cout << "Recv [" << self.get() << "] : " << self->buffer.data() << std::endl;
				std::string sendData{ self->buffer.data(), RecvSize };
				self->SendTo(sendData, RecvSize);
			});
	}

	void Send_Internal(std::size_t Length)
	{
		socket.async_send_to(
			boost::asio::buffer(buffer.data(), Length), remoteEndpoint,
			[self = shared_from_this()](boost::system::error_code ErrCode, std::size_t SendSize) 
			{
				if (ErrCode)
				{
					std::cout << "Send Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
					return;
				}
				std::string sendData{ self->buffer.data(), SendSize };
				std::cout << "Send [" << self.get() << "] : " << sendData << std::endl;
				self->RecvFrom();
			});
	}

private:
	boost::asio::ip::udp::socket socket;
	boost::asio::ip::udp::endpoint remoteEndpoint{};
	std::array<char, BUFFER_SIZE> buffer{};
};