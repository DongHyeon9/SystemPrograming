#pragma once
#include "AppBase.h"

class NETWORK_API Client_UDP : public AppBase<Client_UDP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

