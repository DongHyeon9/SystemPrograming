#pragma once
#include "AppBase.h"

class NETWORK_API Client_RIO : public AppBase<Client_RIO>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

};

