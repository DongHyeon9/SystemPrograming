#pragma once
#include "AppBase.h"

class NETWORK_API Client_Asio_AsyncUDP : public AppBase<Client_Asio_AsyncUDP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	void DoSend();
	void Send_Internal(const boost::system::error_code& ErrCode, std::size_t BytesTransferred);
	void DoReceive();
	void Receive_Internal(const boost::system::error_code& ErrCode, std::size_t BytesTransferred);

private:
	// 생성자에서 변수 선언 순서에 의존하는 코드는 좋은 코드가 아니다...
	boost::asio::io_context ctx{};
	boost::asio::ip::udp::socket socket{ ctx };

	boost::asio::ip::udp::endpoint remoteEndpoint{};

	std::thread ioThread{};
	std::string buffer{};
};

