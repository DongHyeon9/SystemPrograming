#pragma once
#include "AppBase.h"

class NETWORK_API Server_Asio_AsyncTCP : public AppBase<Server_Asio_AsyncTCP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	void DoAccept();

private:
	boost::asio::io_context ctx{};
	boost::asio::ip::tcp::acceptor acceptor{ ctx };
	boost::asio::ip::tcp::endpoint endpoint{};
	std::thread ioThread{};

	std::list<std::shared_ptr<Session_Asio<E_PROTOCOL_TYPE::TCP>>> sessionList{};
};