#include "Client_TCP.h"
#include "Client_UDP.h"
#include "Client_NBS.h"
#include "Client_Select.h"
#include "Client_WSAEventSelect.h"
#include "Client_Overlapped_Event.h"
#include "Client_Overlapped_Callback.h"
#include "Client_WSAAsyncSelect.h"
#include "Client_IOCP.h"

int main()
{
	// Client_TCP::GetInstance()->Initialize();
	// Client_TCP::GetInstance()->Run();
	// Client_TCP::GetInstance()->Deinitialize();

	//Client_UDP::GetInstance()->Initialize();
	//Client_UDP::GetInstance()->Run();
	//Client_UDP::GetInstance()->Deinitialize();

	//Client_NBS::GetInstance()->Initialize();
	//Client_NBS::GetInstance()->Run();
	//Client_NBS::GetInstance()->Deinitialize();

	//Client_Select::GetInstance()->Initialize();
	//Client_Select::GetInstance()->Run();
	//Client_Select::GetInstance()->Deinitialize();

	//Client_WSAEventSelect::GetInstance()->Initialize();
	//Client_WSAEventSelect::GetInstance()->Run();
	//Client_WSAEventSelect::GetInstance()->Deinitialize();

	//Client_Overlapped_Event::GetInstance()->Initialize();
	//Client_Overlapped_Event::GetInstance()->Run();
	//Client_Overlapped_Event::GetInstance()->Deinitialize();

	//Client_Overlapped_Callback::GetInstance()->Initialize();
	//Client_Overlapped_Callback::GetInstance()->Run();
	//Client_Overlapped_Callback::GetInstance()->Deinitialize();

	//Client_WSAAsyncSelect::GetInstance()->Initialize();
	//Client_WSAAsyncSelect::GetInstance()->Run();
	//Client_WSAAsyncSelect::GetInstance()->Deinitialize();

	Client_IOCP::GetInstance()->Initialize();
	Client_IOCP::GetInstance()->Run();
	Client_IOCP::GetInstance()->Deinitialize();
}