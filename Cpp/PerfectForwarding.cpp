#include "PerfectForwarding.h"

void ExampleFunc::Func1(int n)
{
	LogFunc();
	std::this_thread::sleep_for(std::chrono::seconds(n));
}

void ExampleFunc::Func2(int n)
{
	LogFunc();
}

void ExampleFunc::Func3(int& n)
{
	LogFunc();
	n = 30;
}

void ExampleFunc::Func4(int&& n)
{
	LogFunc();
}

int& ExampleFunc::Func5(int& n)
{
	LogFunc();
	n = 50;
	return n;
}

void ExampleFunc::Func6(int a)
{
	LogFunc();
}

void ExampleFunc::Func6(int a, int b)
{
	LogFunc();
}

void ExampleMain()
{
	// 1. 시간측정
	int n{};
	Chronometry(ExampleFunc::Func1, 1);

	// 2. 인자변경
	PerfectForwarding(ExampleFunc::Func2, 2);
	PerfectForwarding(ExampleFunc::Func3, n);
	PerfectForwarding(ExampleFunc::Func4, 4);
	std::println("{}", n);

	// 3. return값
	int& ret{ PerfectForwarding(ExampleFunc::Func5, n) };
	std::println("{}", n);
	ret = 70;
	std::println("{}", n);
	std::println("{}", ret);

	// 4. 오버로딩
	PerfectForwarding(static_cast<void(*)(int)>(ExampleFunc::Func6), 1);
	PerfectForwarding(static_cast<void(*)(int, int)>(ExampleFunc::Func6), 2, 3);

	// 5. 맴버 함수
	Foo f1{};
	const Foo f2{};
	PerfectForwarding(static_cast<void(Foo::*)(int)>(&Foo::Bar1), f1, 1);
	PerfectForwarding(static_cast<void(Foo::*)(int, int)>(&Foo::Bar1), Foo{}, 2, 3);
	PerfectForwarding(&Foo::Bar2, f2);

	// 6. 함수자
	Functor func{};
	PerfectForwarding(Functor(), 1);
	PerfectForwarding(func, 2);
}

void Foo::Bar1(int a)
{
	LogFunc();
}

void Foo::Bar1(int a, int b)
{
	LogFunc();
}

void Foo::Bar2() const
{
	LogFunc();
}
