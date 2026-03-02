#include "Template.h"

// template parameter
// 템플릿의 인자로 템플릿,함수포인터, 주소값 등도 넘길 수 있다
// 단, 주소값은 전역변수만 가능
template<class T1, class T2>
struct Foo1
{

};

void Func1(int)
{

}

template<class T,
	void (*FP)(int),
	int* P,
	template<class, class> class C = Foo1 >
struct Tempate1
{

};

// dependant name
// 템플릿 내부에 선언된 템플릿엔 template임을 명시해 줘야된다
template<class T>
struct Template2
{
	using type = T;

	template<class U>
	struct Other {};

	template<class U>
	void Func(type) {}
};

template<class T>
void Func2()
{
	T obj{};
	typename T::type v1{};
	typename T::template Other<T> v2{};
	obj.template Func<T>(v1);
}

// coercion by member template
template<class T1, class T2>
class Template3
{
public:
	Template3(const T1& Arg1, const T2& Arg2) :data1{ Arg1 }, data2{ Arg2 } {}

	template<class, class> friend class Template3;

	template<class U1, class U2>
	Template3(const Template3<U1, U2>& Rhs) :data1{ static_cast<T1>(Rhs.data1) }, data2{ static_cast<T2>(Rhs.data2) } {}

private:
	T1 data1{};
	T2 data2{};
};

using uint64 = unsigned long long;

// template meta programing
template<uint64 N>
struct fibonacci
{
	static constexpr uint64 value = N * fibonacci<N - 1>::value;
};

template<>
struct fibonacci<1>
{
	static constexpr uint64 value = 1;
};

template<>
struct fibonacci<0>
{
	static constexpr uint64 value = 1;
};

template<uint64 N>
constexpr uint64 fibonacci_v = fibonacci<N>::value;

// integral_constant
template<class T, T N>
struct integral_constant
{
	static constexpr T value = N;
	using value_type = T;
	using type = integral_constant;
	constexpr operator value_type() const noexcept { return value; }
	constexpr value_type operator()() const noexcept { return value; }
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template<class T> struct is_pointer :public false_type {};
template<class T> struct is_pointer<T*> :public true_type {};

template<class T>
void PrintImpl(T val, true_type)
{
	std::cout << "Pointer : " << *val << std::endl;
}

template<class T>
void PrintImpl(T val, false_type)
{
	std::cout << "Value : " << val << std::endl;
}

template<class T>
void Print(const T& val)
{
	PrintImpl(val, is_pointer<T>{});
}

//concept
// 템플릿 인자에 대한 제약들을 정의하는 template
template<class T>
concept container = requires(T & C)
{
	//{ std::begin(C) }->std::input_or_output_iterator;
	//{ std::end(C) }->std::input_or_output_iterator;

	std::ranges::begin(C);
	std::ranges::end(C);
};

template<class T>
concept default_constructor = requires
{
	T{};
};

template<class T>
concept dereferencing = requires(T A)
{
	*A;
};

template<class T>
concept C = requires(T Lhs, T Rhs)
{
	// Simple Requirement
	Lhs + Rhs;
	Lhs.begin();

	// Type Requirement
	typename T::value_type;

	// Compound Requirement
	// 두객체를 더할 수 있어야되고,
	// 예외를 발생시키면 안되며(optional)
	// 리턴값이 int형이어야 된다(optional) -> 여기엔 concept만 적을 수 있음
	{ Lhs + Rhs } noexcept->std::same_as<int>;

	// Nested Requirement, requires expression(표현식)
	// requires안에 requires
	// concept만 가능
	// sizeof(T) > 4;로 쓸경우 sizeof(T)연산이 가능해야된다는 뜻으로 해석한다(의도와 다름)
		requires sizeof(T) > 4;
	// 스마트포인터가아닌 진짜 포인터를 판별하는 concept
		requires std::same_as<std::remove_reference_t<decltype(*Lhs)>, std::remove_pointer_t<T>>;
};

template<class T>
void Func3(const T& Arg)
{
	if constexpr (container<T>)
	{
		std::cout << "Container" << std::endl;
	}
	else if constexpr (std::input_or_output_iterator<T>)
	{
		std::cout << "Iterator" << std::endl;
	}
}

template<class T>
void Func4(const T& Arg)
{
	constexpr bool bIsAddable{ requires(T Lhs,T Rhs) { Lhs + Rhs; } };
	if constexpr (bIsAddable)
	{
		std::cout << "Is Addable" << std::endl;
	}
	else
	{
		std::cout << "Is Not Addable" << std::endl;
	}
}

// requires clauses(절)
// concept과 typetrait 등 컴파일 시간에 결정될 수 있는 bool값 다 가능
template<class T> requires (
	(sizeof(T) > 4) &&
	default_constructor<T>&&
	requires(T Lhs, T Rhs) { { Lhs + Rhs }->std::same_as<T>; })
void Func5(const T& Arg)
{

}

// variadic template
template<class ... Ts>
void Func6(const Ts& ... Args)
{
	std::cout << sizeof...(Ts) << std::endl;
	std::cout << sizeof...(Args) << std::endl;
	// Pack Expansion
	// Parameter Pack에 있는 모든 요소를 ,를 사용해서 순서대로 나열
	// 함수 인자를 전달하는 괄호 안, {}로 초기화되는 문맥에서만 사용가능
	// Args...;								// 1,2,3
	// ++Args...;							// ++1,++2,++3
	// std::abs(Args...);					// std::abs(1,2,3)
	// std::abs(Args)...;					// std::abs(1),std::abs(2),std::abs(3)
	int dummy[]{ 0,(Print(Args),0)... };	//{ (Print(1),0), (Print(2),0), (Print(3),0) }
}

template<class ... Ts>
struct Outer
{
	template<Ts ... Value>
	struct Inner {};
};

Outer<int, double>::Inner<3, 3.14> in{};

class A {};
class B {};

template<class ...Ts>
class X :public Ts...	// class X : public A, public B
{
public:
	X(const Ts&... Args) :Ts(Args)...{}	// : A(Arg1), B(Arg2) {}
};

// C++17 이전
void Print_Var() { std::cout << std::endl; }

template<class T, class ...Ts>
void Print_Var(const T& Value, const Ts& ...Args)
{
	std::cout << Value << " ";
	// C++ 17부터
	if constexpr (sizeof...(Args) > 0)
		Print_Var(Args...);
}

// fold expression
// 파라미터 팩에 있는 모든요소에 이항 연산을 적용
template<class ...Ts>
auto Sum(Ts...Args)
{
	// 인자가 없으면 에러
	//return (... + Args);		// (((((1)+2)+3)+4)+5)

	// Unary Right Fold
	// (args+...) = (e1+(e2+(e3+(e4+(e5)))))

	// Unary Left Fold
	// (...+args) = (((((e1)+e2)+e3)+e4)+e5)

	// Binary Right Fold
	// (args+...+0) (e1+(e2+(e3+(e4+(e5+0)))))

	// Binary Left Fold
	// (0+args+...) (((((0+e1)+e2)+e3)+e4)+e5)

	(std::cout << ... << Args);
	return (0 + ... + Args);
}

// Tuple
template<class ...Ts>
struct Tuple
{
	static constexpr size_t N = 0;
};

template<class T, class ...Ts>
struct Tuple<T, Ts...> : public Tuple<Ts...>
{
	using Base = Tuple<Ts...>;
	T value{};

	Tuple() = default;

	template<class A, class ...Types>
	Tuple(A&& Value, Types&& ...Args) :Base{ std::forward<Types>(Args)... }, value{ std::forward<A>(Value) } {}

	static constexpr size_t N = Base::N + 1;
};

template<std::size_t N, class TP>
struct tuple_element;

template<class T, class ...Ts>
struct tuple_element<0, Tuple<T, Ts...>>
{
	// Elem의 타입
	using type = T;
	// Tuple의 타입
	using tuple_type = Tuple<T, Ts...>;
};

template<std::size_t N, class T, class ...Ts>
struct tuple_element<N, Tuple<T, Ts...>>
{
	using type = typename tuple_element<N - 1, Tuple<Ts...>>::type;
	using tuple_type = typename tuple_element<N - 1, Tuple<Ts...>>::tuple_type;
};

template<std::size_t N, class TP>
typename tuple_element<N, TP>::type& get(TP& T)
{
	return static_cast<typename tuple_element<N, TP>::tuple_type&>(T).value;
}

template<class T, T...Ints>
struct integer_sequence
{
	static_assert(std::is_integral_v<T>, "Not Integral");

	using value_type = T;

	static constexpr size_t size()noexcept { return sizeof...(Ints); }
};

template<std::size_t ...Idx>
using index_sequence = integer_sequence<std::size_t, Idx...>;

template<class T, T I, T N, T... xs>
struct make_seq_impl
{
	using type = typename make_seq_impl<T, I + 1, N, xs..., I>::type;
};

template<class T, T N, T... xs>
struct make_seq_impl<T, N, N, xs...>
{
	using type = integer_sequence<T, xs...>;
};

template<class T, T N>
using make_integer_sequence = typename make_seq_impl<T, 0, N>::type;

template<std::size_t N>
using make_index_sequence = make_integer_sequence<std::size_t, N>;

template<class Tuple, std::size_t ...Idx>
void Print_Tuple_Impl(Tuple& Tp, integer_sequence<std::size_t, Idx...>)
{
	(std::print("{} ", get<Idx>(Tp)), ...);
	std::cout << std::endl;
}

template<class Tuple, std::size_t ...Idx>
void Print_Tuple(Tuple& Tp)
{
	Print_Tuple_Impl(Tp, make_index_sequence<Tuple::N>{});
}

// SFINAE
// Subscription Failure Is Not An Error
// 치환 실패는 에러가 아니다
// SFINAE는 3가지 경우에서 실패할 경우 적용
// 1. Function return Type
// 2. function parameter
// 3. template parameter

void Func7(...) { std::cout << "..." << std::endl; }

// 1. Function return Type
template<class T>
typename T::type Func7(T Arg) { std::cout << "Return Type" << std::endl; }

// 2. function parameter
template<class T>
void Func7(T Arg1, typename T::type Arg2 = 0) { std::cout << "Param Type" << std::endl; }

// 3. template parameter
template<class T, class U = typename T::type>
void Func7(T Arg) { std::cout << "Template Param Type" << std::endl; }

template<class T, class T::type = 0>
void Func7(T Arg) { std::cout << "Template Param Type" << std::endl; }

// enable_if
template<bool B, class T = void>
struct enable_if
{

};

template<class T>
struct enable_if<true, T>
{
	using type = T;
};

template<bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

template<class T>
enable_if_t<std::is_pointer_v<T>> PrintType(const T&)
{
	std::cout << "Pointer" << std::endl;
}

template<class T>
enable_if_t<!std::is_pointer_v<T>> PrintType(const T&)
{
	std::cout << "Value" << std::endl;
}

void ExampleMain()
{
	{
		static int StaticVar{};
		Tempate1<int, Func1, &StaticVar> temp1{};
	}

	{
		Func2<Template2<int>>();
	}

	{
		Template3<float, double> temp1{ 1.1f,2.2 };
		Template3<int, long long> temp2{ temp1 };
	}

	{
		std::cout << fibonacci_v<5> << std::endl;
	}

	{
		int v{ 30 };
		int* p{ &v };
		Print(v);
		Print(p);
	}

	{
		std::vector v{ 1,2,3,4,5 };
		Func3(v);
		Func3(v.begin());

		Func4(v);
		Func4(std::string{});

		class Foo2 { Foo2() = delete; };
		std::cout << default_constructor<Foo2> << ", " << default_constructor<int> << std::endl;
		std::cout << dereferencing<int> << ", " << dereferencing<int*> << std::endl;
	}

	{
		Func6(3, 3.14, 'A');
		A a{};
		B b{};
		X<A, B> x{ a,b };
		Print_Var(1, 2, 3, 4, 5);
		std::cout << std::endl << Sum(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) << std::endl;
	}

	{
		Tuple<int, double, char> tuple1{ 3, 3.4, 'A' };
		Tuple<int, double, char> tuple2{ 3, 3.4 };

		std::cout << get<0>(tuple1) << ", " << get<1>(tuple1) << ", " << get<2>(tuple1) << std::endl;

		get<0>(tuple1) = 99;
		get<1>(tuple1) = 3.14;
		Print_Tuple(tuple1);
	}

	{
		Func7(3.14);
		int* a{};
		PrintType(a);
		PrintType(30);
	}
}