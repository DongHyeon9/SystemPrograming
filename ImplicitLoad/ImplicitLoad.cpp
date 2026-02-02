#include <Windows.h>
#include <process.h>
#include <iostream>
#include <tchar.h>

#pragma comment(lib,"..\\x64\\Debug\\TestDll.lib")

__declspec(dllimport) void WINAPI DllFunc(int);

struct ThreadParam
{
    int param1{};
};

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