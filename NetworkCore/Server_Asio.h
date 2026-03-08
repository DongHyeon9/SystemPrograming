#pragma once
#include "AppBase.h"

class NETWORK_API Server_Asio : public AppBase<Server_Asio>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

};

