#pragma once
#include "AppBase.h"

// 논블로킹 방식의 TCP 소켓모델
class NETWORK_API Server_NBS : public AppBase<Server_NBS>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
	SOCKET clientSocket{};
};

