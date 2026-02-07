#pragma once
#include "AppBase.h"

class NETWORK_API Server_UDP : public AppBase<Server_UDP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

