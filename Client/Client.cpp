#include "Client_TCP.h"
#include "Client_UDP.h"

int main()
{
	// Client_TCP::GetInstance()->Initialize();
	// Client_TCP::GetInstance()->Run();
	// Client_TCP::GetInstance()->Deinitialize();

	Client_UDP::GetInstance()->Initialize();
	Client_UDP::GetInstance()->Run();
	Client_UDP::GetInstance()->Deinitialize();
}