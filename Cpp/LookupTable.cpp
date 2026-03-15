#include "LookupTable.h"

enum class LUT_TYPE : uint8_t
{
	ADD,
	SUB,
	MUL,
	DIV,

	END,
};

// std::function을 사용한 LUT
// 장점
// - 유연함 (std::function은 다양한 Callable Object를 저장할 수 있음 -> 함수 포인터, 람다, std::bind 결과 등)
// 단점
// - 성능 저하 (std::function은 내부적으로 가상 함수 호출과 유사한 방식으로 동작)
class LUT_Function
{
public:
	LUT_Function()
	{
		functions =
		{
			std::bind(&LUT_Function::Add, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&LUT_Function::Sub, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&LUT_Function::Mul, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&LUT_Function::Div, this, std::placeholders::_1, std::placeholders::_2),
		};
	}

	int Execute(LUT_TYPE type, int a, int b)
	{
		return functions[static_cast<uint8_t>(type)](a, b);
	}

private:
	int Add(int a, int b) { return a + b; }
	int Sub(int a, int b) { return a - b; }
	int Mul(int a, int b) { return a * b; }
	int Div(int a, int b) { return a / b; }

	std::array<std::function<int(int, int)>, static_cast<uint8_t>(LUT_TYPE::END)> functions{};
};

// 멤버 함수 포인터를 사용한 LUT
// 장점
// - 위에꺼보단 빠름
// 단점
// - 여전히 멤버 함수 포인터는 일반 함수 포인터보다 호출이 느릴 수 있음
// - 위에꺼 보다 덜 유연함
class LUT_Memfunc
{
public:
	using Func = int(LUT_Memfunc::*)(int, int);

	int Execute(LUT_TYPE type, int a, int b)
	{
		return (this->*functions[static_cast<uint8_t>(type)])(a, b);
	}

private:
	int Add(int a, int b) { return a + b; }
	int Sub(int a, int b) { return a - b; }
	int Mul(int a, int b) { return a * b; }
	int Div(int a, int b) { return a / b; }

	inline static constexpr std::array<Func, static_cast<uint8_t>(LUT_TYPE::END)> functions
	{
		&LUT_Memfunc::Add,
		&LUT_Memfunc::Sub,
		&LUT_Memfunc::Mul,
		&LUT_Memfunc::Div,
	};
};

// 일반 함수 포인터를 사용한 LUT
// 장점
// - 가장 빠름 (일반 함수 포인터는 직접 호출되므로 오버헤드가 거의 없음)
// 단점
// - 가장 덜 유연함 (일반 함수 포인터는 멤버 함수나 람다 등을 저장할 수 없음)
namespace LUT
{
	int Add(int a, int b) { return a + b; }
	int Sub(int a, int b) { return a - b; }
	int Mul(int a, int b) { return a * b; }
	int Div(int a, int b) { return a / b; }

	inline static constexpr std::array<int(*)(int, int), static_cast<uint8_t>(LUT_TYPE::END)> functions
	{
		&Add,
		&Sub,
		&Mul,
		&Div,
	};

	int Execute(LUT_TYPE type, int a, int b)
	{
		return functions[static_cast<uint8_t>(type)](a, b);
	}
}

// Callable
// - 함수
// - 함수 포인터
// - 멤버 함수 포인터
// - 맴버 데이터 포인터
// - 펑터
// - 람다

// std::bind
// 함수와 인자를 묶어서 새로운 Callable을 만듬 -> 생성

// std::invoke
// 어떤 Callable이든지 일관된 방식으로 호출 -> 실행

void ExampleMain()
{

}