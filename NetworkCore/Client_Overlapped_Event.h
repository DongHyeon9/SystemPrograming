#pragma once
#include "AppBase.h"

class NETWORK_API Client_Overlapped_Event : public AppBase<Client_Overlapped_Event>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

