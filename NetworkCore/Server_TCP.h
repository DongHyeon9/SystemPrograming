#pragma once
#include "AppBase.h"

class NETWORK_API Server_TCP : public AppBase<Server_TCP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};

	SOCKET clientSocket{};
};

