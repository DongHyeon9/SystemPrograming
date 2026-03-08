#pragma once
#include "AppBase.h"

class NETWORK_API Client_Asio : public AppBase<Client_Asio>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

};

