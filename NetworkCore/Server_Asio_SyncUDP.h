#pragma once
#include "AppBase.h"

class NETWORK_API Server_Asio_SyncUDP : public AppBase<Server_Asio_SyncUDP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	boost::asio::io_context ctx{};
};

