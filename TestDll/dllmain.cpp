#include <Windows.h>
#include <unordered_map>
#include <functional>
#include "FunctionLibrary.h"
#include <tchar.h>

using ENTRY_FUNC_ARGS = void(HMODULE, DWORD, LPVOID);
using ENTRY_FUNC_PAIR = std::pair<uint32_t, std::function<ENTRY_FUNC_ARGS>>;

std::unordered_map<uint32_t, std::function<ENTRY_FUNC_ARGS>> ENTRY_FUNC_LUT
{
	ENTRY_FUNC_PAIR{DLL_PROCESS_ATTACH, OnProcessAttach},
	ENTRY_FUNC_PAIR{DLL_PROCESS_DETACH, OnProcessDetach},
	ENTRY_FUNC_PAIR{DLL_THREAD_ATTACH, OnThreadAttach},
	ENTRY_FUNC_PAIR{DLL_THREAD_DETACH, OnThreadDetach},
};

__declspec(dllexport) void WINAPI DllFunc(int);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ENTRY_FUNC_LUT.find(ul_reason_for_call) != ENTRY_FUNC_LUT.end())
	{
		ENTRY_FUNC_LUT[ul_reason_for_call](hModule, ul_reason_for_call, lpReserved);
	}
	else
	{
		// 예외처리
		return FALSE;
	}

	return TRUE;
}

void WINAPI DllFunc(int Param)
{
	::_tprintf(TEXT("Dll Func : %d\n"), Param);
}