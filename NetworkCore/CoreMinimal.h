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
