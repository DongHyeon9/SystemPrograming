#pragma once
#include "Functions.h"

template<class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		static T inst{};
		return &inst;
	}

	Singleton(const Singleton<T>&) = delete;
	Singleton<T>& operator=(const Singleton<T>&) = delete;

protected:
	Singleton() = default;
};

namespace NETWORK_INTERFACE
{
	extern NETWORK_API CREATE_SOCKET CREATE_SOCKET_FUNC;
	extern NETWORK_API CONNECT CONNECT_FUNC;
	extern NETWORK_API BIND BIND_FUNC;
	extern NETWORK_API LISTEN LISTEN_FUNC;
	extern NETWORK_API ACCEPT ACCEPT_FUNC;
	extern NETWORK_API SEND SEND_FUNC;
	extern NETWORK_API RECV RECV_FUNC;
}