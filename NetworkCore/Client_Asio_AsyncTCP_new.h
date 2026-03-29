#pragma once
#include "AppBase.h"

class NETWORK_API Client_Asio_AsyncTCP : public AppBase<Client_Asio_AsyncTCP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	void DoSend();
	void HandleSend(const boost::system::error_code& errCode, std::size_t bytesTransferred);
	void DoReceive();
	void HandleReceive(const boost::system::error_code& errCode, std::size_t bytesTransferred);

private:
	boost::asio::io_context ctx{};
	boost::asio::ip::tcp::socket socket{ ctx };
	std::thread ioThread;
	std::atomic<int> sendCount{ 0 };
	std::array<char, BUFFER_SIZE> receiveBuffer{};
};
