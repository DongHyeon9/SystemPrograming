#pragma once
#include "AppBase.h"

class NETWORK_API Client_IOCP : public AppBase<Client_IOCP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

