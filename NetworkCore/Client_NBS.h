#pragma once
#include "AppBase.h"

class NETWORK_API Client_NBS : public AppBase<Client_NBS>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

