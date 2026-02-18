#pragma once
#include "AppBase.h"

class NETWORK_API Client_WSAAsyncSelect : public AppBase<Client_WSAAsyncSelect>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

