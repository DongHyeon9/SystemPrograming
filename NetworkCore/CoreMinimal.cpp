#include "CoreMinimal.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

namespace NETWORK_INTERFACE
{
    CREATE_SOCKET CREATE_SOCKET_FUNC{};
    CONNECT CONNECT_FUNC{};
    BIND BIND_FUNC{};
    LISTEN LISTEN_FUNC{};
    ACCEPT ACCEPT_FUNC{};
    SEND SEND_FUNC{};
    RECV RECV_FUNC{};
}
