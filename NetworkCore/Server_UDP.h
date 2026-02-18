#pragma once
#include "AppBase.h"

// 블로킹 방식의 UDP 소켓모델
class NETWORK_API Server_UDP : public AppBase<Server_UDP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

