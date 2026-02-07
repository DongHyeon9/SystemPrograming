#include "Server_TCP.h"
#include "Server_UDP.h"

int main()
{
	// Server_TCP::GetInstance()->Initialize();
	// Server_TCP::GetInstance()->Run();
	// Server_TCP::GetInstance()->Deinitialize();

	Server_UDP::GetInstance()->Initialize();
	Server_UDP::GetInstance()->Run();
	Server_UDP::GetInstance()->Deinitialize();
}