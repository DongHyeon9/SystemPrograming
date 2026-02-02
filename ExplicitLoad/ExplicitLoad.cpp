#include <Windows.h>
#include <process.h>
#include <iostream>
#include <tchar.h>

struct ThreadParam
{
    int param1{};
};

using DLL_FUNC = void (WINAPI*) (int);

DLL_FUNC DllFunc{};

unsigned __stdcall ThreadFunc(void* Arg)
{
    ::_tprintf(TEXT("Thread Func Start\n"));
    auto param = (ThreadParam*)Arg;
    DllFunc(param->param1);
    ::_tprintf(TEXT("Thread Func End\n"));
    return 0;
}

int main()
{
    HMODULE hDll = ::LoadLibrary(TEXT("TestDll.dll"));
    // 네임맹글링 -> 이게 싫다면 extern "C"를 붙여야됨
    DllFunc = (DLL_FUNC)::GetProcAddress(hDll, "?DllFunc@@YAXH@Z");

    HANDLE hThread{};
    ThreadParam param{};
    param.param1 = 10;

    unsigned threadID{};

    hThread = (HANDLE)_beginthreadex(NULL, 0, &ThreadFunc, &param, 0, &threadID);

    ::_tprintf(TEXT("Thread ID : %d\n"), threadID);

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    ::_tprintf(TEXT("Main End\n"));
}