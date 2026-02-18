#pragma once

#if NET_WORK_CORE
    #define NETWORK_API __declspec(dllexport)
#else
    #define NETWORK_API __declspec(dllimport)
#endif

//warning C4251: 'Session::buffer': 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>'에서는 'Session'의 클라이언트에서 DLL 인터페이스를 사용하도록 지정해야 함
#pragma warning(disable : 4251)

#define GET_PROC_ADDRESS(memFunc, funcType, funcName) memFunc = reinterpret_cast<funcType>(GetProcAddress(module, funcName)); \
    if (!memFunc) { std::cout << "Fail GetProcAddress " << funcName << " : " << ::GetLastError() << std::endl; return false; }