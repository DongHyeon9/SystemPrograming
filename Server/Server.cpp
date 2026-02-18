#include "Server_TCP.h"
#include "Server_UDP.h"
#include "Server_NBS.h"
#include "Server_Select.h"
#include "Server_WSAEventSelect.h"
#include "Server_Overlapped_Event.h"
#include "Server_Overlapped_Callback.h"
#include "Server_WSAAsyncSelect.h"
#include "Server_IOCP.h"

int main()
{
	// Server_TCP::GetInstance()->Initialize();
	// Server_TCP::GetInstance()->Run();
	// Server_TCP::GetInstance()->Deinitialize();

	//Server_UDP::GetInstance()->Initialize();
	//Server_UDP::GetInstance()->Run();
	//Server_UDP::GetInstance()->Deinitialize();

	//Server_NBS::GetInstance()->Initialize();
	//Server_NBS::GetInstance()->Run();
	//Server_NBS::GetInstance()->Deinitialize();

	//Server_Select::GetInstance()->Initialize();
	//Server_Select::GetInstance()->Run();
	//Server_Select::GetInstance()->Deinitialize();

	//Server_WSAEventSelect::GetInstance()->Initialize();
	//Server_WSAEventSelect::GetInstance()->Run();
	//Server_WSAEventSelect::GetInstance()->Deinitialize();

	//Server_Overlapped_Event::GetInstance()->Initialize();
	//Server_Overlapped_Event::GetInstance()->Run();
	//Server_Overlapped_Event::GetInstance()->Deinitialize();

	//Server_Overlapped_Callback::GetInstance()->Initialize();
	//Server_Overlapped_Callback::GetInstance()->Run();
	//Server_Overlapped_Callback::GetInstance()->Deinitialize();

	//Server_WSAAsyncSelect::GetInstance()->Initialize();
	//Server_WSAAsyncSelect::GetInstance()->Run();
	//Server_WSAAsyncSelect::GetInstance()->Deinitialize();

	Server_IOCP::GetInstance()->Initialize();
	Server_IOCP::GetInstance()->Run();
	Server_IOCP::GetInstance()->Deinitialize();
}