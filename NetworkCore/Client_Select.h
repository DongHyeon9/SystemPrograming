#pragma once
#include "AppBase.h"

class NETWORK_API Client_Select : public AppBase<Client_Select>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

