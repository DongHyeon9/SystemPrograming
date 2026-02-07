#pragma once
#include "CoreMinimal.h"

template<class T>
class AppBase : public Singleton<T>
{
public:
	virtual bool Initialize()
    {
        WSAData wsaData{};
        if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            std::cout << "WSAStartup Fail!" << std::endl;
            return false;
        }

        return true;
    }

	virtual void Deinitialize()
    {
        ::WSACleanup();
    }

	virtual void Run()abstract;
};

