#include "FunctionLibrary.h"
#include <tchar.h>

void OnProcessAttach(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	::_tprintf(_T("=====On Process Attach=====\n"));
	::MessageBox(0, _T("On Attach!"), _T("Test Dll"), 0);
	::_tprintf(_T("Dll Address : %p\n"), hModule);
	::_tprintf(_T("=====On Process Attach=====\n"));
}

void OnProcessDetach(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	::_tprintf(_T("=====On Process Detach=====\n"));
}

void OnThreadAttach(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	::_tprintf(_T("=====On Thread Attach=====\n"));
	::_tprintf(_T("Thread ID : %d\n"), GetCurrentThreadId());
	::_tprintf(_T("=====On Thread Attach=====\n"));
}

void OnThreadDetach(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	::_tprintf(_T("=====On Thread Detach=====\n"));
	::_tprintf(_T("Thread ID : %d\n"), GetCurrentThreadId());
	::_tprintf(_T("=====On Thread Detach=====\n"));
}
