#pragma once
#include "AppBase.h"

class NETWORK_API Client_TCP : public AppBase<Client_TCP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET clientSocket{};
};

