#pragma once
#include "AppBase.h"

class NETWORK_API Server_Asio_AsyncUDP : public AppBase<Server_Asio_AsyncUDP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	boost::asio::io_context ctx{};
	boost::asio::ip::udp::socket socket{ ctx };
	boost::asio::ip::udp::endpoint endpoint{};

	std::thread ioThread{};

	std::shared_ptr<Session_Asio<E_PROTOCOL_TYPE::UDP>> session{};
};