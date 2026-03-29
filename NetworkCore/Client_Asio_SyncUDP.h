#pragma once
#include "AppBase.h"

class NETWORK_API Client_Asio_SyncUDP : public AppBase<Client_Asio_SyncUDP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	boost::asio::io_context ctx{};
};

