#include "TypeDeduction.h"

void ExampleMain()
{
	// auto
	// 템플리이랑 타입 추론이 거의 동일함
	// auto - Unifrom initialization : std::initializer_list로 추론
	// template - Unifrom initialization : 추론불가
	// reference collapsing 규칙이 적용된다
	// 참고 : https://www.ibm.com/docs/en/xl-c-and-cpp-aix/16.1.0?topic=operators-reference-collapsing-c11
	// auto, T = const, volatile, reference 속성이 제거된다
	// auto&, T& = reference 속성이 제거되고 다시 reference가 붙음 const, volatile이 유지된다
	// auto&&, T&& = forwarding reference 규칙이 적용된다
	
	// forwarding reference
	// lvalue 참조와 rvalue 참조를 모두 받을 수 있는 참조
	// lvalue = lvalue reference
	// rvalue = rvalue reference
	{
		int x{ 10 };

		auto&& a = x; // a는 int&로 추론됨
		auto&& b = 20; // b는 int&&로 추론됨
	}
	
	// decltype
	// 표현식의 타입을 그대로 추론하는 키워드
	{
		// 1. 식별자
		int x{ 10 };
		decltype(x) a = x; // a는 int로 추론됨

		// 2. 괄호로 감싼 식별자
		decltype((x)) b = x; // b는 int&로 추론됨

		// 3. 함수 호출 표현식
		auto Func = []() -> int { return 10; };
		decltype(Func()) c = Func(); // c는 int로 추론됨

		// 4. decltype(auto)
		auto&& d = x; // d는 int&로 추론됨
		decltype(auto) e = x; // e는 int로 추론됨

		// 5. prvalue, xvalue
		decltype(10) f = 10; // f는 int로 추론됨 (prvalue)
		decltype((std::move(x))) j = std::move(x); // j는 int&&로 추론됨 (xvalue)

		// 활용
		// 후위 반환타입을 사용할 때 decltype(auto)를 사용하면 반환 타입을 자동으로 추론할 수 있다
		auto AddFunc = [](auto&& Arg1, auto&& Arg2) -> decltype(auto) { return Arg1 + Arg2; };
	}

	// 런타임 코드
	// 1. new, delete
	// 2. virtual dispatch
	// 3. IO
	// 4. mutex
	// 5. system call
	// 위와 같은 동작을 함수 내부에서 취하지 않고
	// 
	// 1. pure function : 함수의 반환값과 매개변수의 타입이 고정되어있고, 함수 내부에서 런타임 코드를 취하지 않는 함수
	// 2. small function : 함수의 크기가 작아서 인라인이 가능한 함수
	// 3. getter : 함수 내부에서 런타임 코드를 취하지 않고, 반환값이 고정되어있는 함수
	// 4. math : 함수 내부에서 런타임 코드를 취하지 않고, 반환값이 고정되어있는 수학적인 함수
	// 
	// 위의 조건을 만족하는 함수는 constexpr로 선언해 성능향상을 기대할 수 있다
	// 
	// ex) [[nodiscard]] constexpr auto GetSize() const noexcept -> decltype(auto) { return size; }

	// std::move : 주어진 인수를 무조건 우측값으로 캐스팅
	// std::forard : 주어진 인수를 Rvalue reference인 경우(T&&)에만 우측값으로 캐스팅
}