#include "Functions.h"
#pragma comment(lib, "NetworkCore.lib")

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        NETWORK_INTERFACE::CREATE_SOCKET_FUNC = CreateSocket;
        NETWORK_INTERFACE::CONNECT_FUNC = Connect;
        NETWORK_INTERFACE::BIND_FUNC = Bind;
        NETWORK_INTERFACE::LISTEN_FUNC = Listen;
        NETWORK_INTERFACE::ACCEPT_FUNC = Accept;
        NETWORK_INTERFACE::SEND_FUNC = Send;
        NETWORK_INTERFACE::RECV_FUNC = Recv;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

