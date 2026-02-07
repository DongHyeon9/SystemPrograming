// C++ 23
// msvc 143
// visual studio 2022

#pragma once
#include "CoreMinimal.h"

// 완벽전달
template<class FUNC, class ...ARGS>
decltype(auto) PerfectForwarding(FUNC&& Func, ARGS&& ...Args)
{
	return std::invoke(std::forward<FUNC>(Func), std::forward<ARGS>(Args)...);
}

template<class FUNC, class ...ARGS>
decltype(auto) Chronometry(FUNC&& Func, ARGS&& ...Args)
{
	Benchmark b{};
	return std::invoke(std::forward<FUNC>(Func), std::forward<ARGS>(Args)...);
}

namespace ExampleFunc
{
	// 시간 측정
	void Func1(int n);

	// 인자변경
	void Func2(int n);
	void Func3(int& n);
	void Func4(int&& n);

	// Return값
	int& Func5(int& n);

	// 오버로딩
	void Func6(int a);
	void Func6(int a, int b);
}

class Foo
{
public:
	// 맴버 함수
	void Bar1(int a);
	void Bar1(int a, int b);

	// 상수 객체 맴버 함수
	void Bar2() const;
};

struct Functor
{
	// 함수자
	int operator ()(int n) & 
	{ 
		std::println("operator() &");
		return n; 
	}
	int operator ()(int n) &&
	{
		std::println("operator() &&");
		return n;
	}
};

void ExampleMain();