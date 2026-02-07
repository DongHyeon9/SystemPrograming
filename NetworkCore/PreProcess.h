#pragma once

#if NET_WORK_CORE
    #define NETWORK_API __declspec(dllexport)
#else
    #define NETWORK_API __declspec(dllimport)
#endif

#define GET_PROC_ADDRESS(memFunc, funcType, funcName) memFunc = reinterpret_cast<funcType>(GetProcAddress(module, funcName)); \
    if (!memFunc) { std::cout << "Fail GetProcAddress " << funcName << " : " << ::GetLastError() << std::endl; return false; }