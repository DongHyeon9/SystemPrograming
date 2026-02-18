#pragma once
#include "AppBase.h"

class NETWORK_API Client_WSAEventSelect : public AppBase<Client_WSAEventSelect>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

