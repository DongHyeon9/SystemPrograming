#include "UniquePtr.h"

struct Empty {};
struct Data
{
	// msvc에선 지원하지 않음
	[[no_unique_address]] Empty e{};
	int data{};
};

// 인자 1, 가변인자 O
struct one_and_variadic_arg_t { explicit one_and_variadic_arg_t() = default; };
// 인자 0, 가변인자 O
struct zero_and_variadic_arg_t { explicit zero_and_variadic_arg_t() = default; };

template<class T1, class T2, bool = std::is_empty_v<T1>&& std::is_final_v<T1>>
struct CompressedPair;

template<class T1, class T2>
struct CompressedPair<T1, T2, false>
{
	T1 first{};
	T2 second{};

	constexpr T1& GetFirst() noexcept { return first; }
	constexpr T2& GetSecond() noexcept { return second; }
	constexpr const T1& GetFirst() const noexcept { return first; }
	constexpr const T2& GetSecond() const noexcept { return second; }

	// std::conjunction_v 인자로 들어온 것들이 전부 true면 true를 반환, 하나라도 false면 false를 반환 like &&연산자
	template<class A1, class ...A2>
	constexpr CompressedPair(one_and_variadic_arg_t, A1&& Arg1, A2&& ...Arg2)
		noexcept(std::conjunction_v<std::is_nothrow_constructible<T1, A1>, std::is_nothrow_constructible<T2, A2...>>)
		:first{ std::forward<A1>(Arg1) }, second{ std::forward<A2>(Arg2)... } {
	}

	template<class ...A2>
	constexpr CompressedPair(zero_and_variadic_arg_t, A2&& ...Arg2)
		noexcept(std::conjunction_v<std::is_nothrow_default_constructible<T1>, std::is_nothrow_constructible<T2, A2...>>)
		: first{}, second{ std::forward<A2>(Arg2)... } {
	}
};

// T1이 Empty인 경우
// EBCO 규칙에 따라 상속받으면 Empty Class의 사이즈는 0
template<class T1, class T2>
struct CompressedPair<T1, T2, true> : public T1
{
	T2 second{};

	constexpr T1& GetFirst() noexcept { return *this; }
	constexpr T2& GetSecond() noexcept { return second; }
	constexpr const T1& GetFirst() const noexcept { return *this; }
	constexpr const T2& GetSecond() const noexcept { return second; }

	// std::conjunction_v 인자로 들어온 것들이 전부 true면 true를 반환, 하나라도 false면 false를 반환 like &&연산자
	template<class A1, class ...A2>
	constexpr CompressedPair(one_and_variadic_arg_t, A1&& Arg1, A2&& ...Arg2)
		noexcept(std::conjunction_v<std::is_nothrow_constructible<T1, A1>, std::is_nothrow_constructible<T2, A2...>>)
		:T1{ std::forward<A1>(Arg1) }, second{ std::forward<A2>(Arg2)... } {
	}

	template<class ...A2>
	constexpr CompressedPair(zero_and_variadic_arg_t, A2&& ...Arg2)
		noexcept(std::conjunction_v<std::is_nothrow_default_constructible<T1>, std::is_nothrow_constructible<T2, A2...>>)
		: T1{}, second{ std::forward<A2>(Arg2)... } {
	}
};

struct Freer
{
	void operator()(void* Ptr) const
	{
		std::cout << "Freer" << std::endl;
		std::free(Ptr);
	}
};

template<class T>
struct DefaultDelete
{
	DefaultDelete() = default;
	template<class U>
	DefaultDelete(const DefaultDelete<U>&) {}
	void operator()(T* Ptr) const
	{
		std::cout << "Default Delete" << std::endl;
		delete Ptr;
	}
};

// T[] 부분특수화
template<class T>
struct DefaultDelete<T[]>
{
	DefaultDelete() = default;
	template<class U>
	DefaultDelete(const DefaultDelete<U>&) {}
	void operator()(T* Ptr) const
	{
		std::cout << "Default Delete[]" << std::endl;
		delete[] Ptr;
	}
};

template<class T, class D = DefaultDelete<T>>
class UniquePtr
{
public:
	using Pointer = T*;
	using ElementType = T;
	using DeleterType = D;

	UniquePtr() noexcept :cPair{ zero_and_variadic_arg_t{} } {}
	UniquePtr(std::nullptr_t) noexcept :cPair{ zero_and_variadic_arg_t{} } {}
	// 복사 초기화를 막기위해 explicit
	explicit UniquePtr(Pointer Ptr) noexcept :cPair{ zero_and_variadic_arg_t{}, Ptr } {}
	UniquePtr(Pointer Ptr, const DeleterType& deletor) noexcept :cPair{ one_and_variadic_arg_t{},deletor, Ptr } {}
	UniquePtr(Pointer Ptr, DeleterType&& deletor) noexcept :cPair{ one_and_variadic_arg_t {},std::move(deletor),Ptr } {}

	UniquePtr(const UniquePtr&) = delete;
	UniquePtr(UniquePtr&& Rhs) noexcept :cPair{ one_and_variadic_arg_t{},std::forward<DeleterType>(Rhs.GetDeleter()),Rhs.Release() } {}

	~UniquePtr() noexcept
	{
		if (cPair.GetSecond())
		{
			cPair.GetFirst()(cPair.GetSecond());
		}
	}

	// 암시적 변환을 막기위해 explicit
	// ex) UniquePtr >> 1;
	explicit operator bool()const noexcept { return static_cast<bool>(cPair.GetSecond()); }

	ElementType& operator*() const noexcept { return *cPair.GetSecond(); }
	Pointer operator->() const noexcept { return cPair.GetSecond(); }

	UniquePtr& operator=(const UniquePtr&) = delete;
	UniquePtr& operator=(UniquePtr&& Rhs) noexcept
	{
		if (this != std::addressof(Rhs))
		{
			reset(Rhs.Release());
			cPair.GetFirst()() = std::forward<DeleterType>(Rhs.cPair.GetFirst());
		}
		return *this;
	}

	// coercion by member template
	// 변환이 가능한지 enable_if로 검사
	// 함수로 접근하는게 아닌 private맴버 변수에 직접 접근한다면 friend를 사용해 줘야한다
	template<class, class>friend class UniquePtr;

	template<class T2, class D2> requires std::is_convertible_v<T2*, T*>&& std::is_convertible_v<D2, D>
	UniquePtr(UniquePtr<T2, D2>&& Rhs) noexcept :cPair{ one_and_variadic_arg_t{},std::forward<D2>(Rhs.GetDeleter()),Rhs.Release() } {}
	template<class T2, class D2> requires std::is_convertible_v<T2*, T*>&& std::is_convertible_v<D2, D>
	UniquePtr& operator=(UniquePtr<T2, D2>&& Rhs) noexcept
	{
		if (this != std::addressof(Rhs))
		{
			reset(Rhs.Release());
			cPair.GetFirst()() = std::forward<D2>(Rhs.cPair.GetFirst());
		}
		return *this;
	}

	Pointer Get()const noexcept { return cPair.GetSecond(); }
	DeleterType& GetDeleter() noexcept { return cPair.GetFirst(); }
	const DeleterType& GetDeleter() const noexcept { return cPair.GetFirst(); }

	void Swap(UniquePtr& Rhs) noexcept
	{
		std::swap(cPair.GetFirst(), Rhs.cPair.GetFirst());
		std::swap(cPair.GetSecond(), Rhs.cPair.GetSecond());
	}

	Pointer Release() noexcept { return std::exchange(cPair.GetSecond(), nullptr); }
	void Reset(Pointer Ptr = nullptr) noexcept
	{
		Pointer old{ std::exchange(cPair.GetSecond(),Ptr) };
		if (old)
			cPair.GetFirst()(old);
	}

private:
	CompressedPair<DeleterType, Pointer> cPair{};

};

// T[]에 대한 부분특수화
template<class T, class D>
class UniquePtr<T[], D>
{
public:
	using Pointer = T*;
	using ElementType = T;
	using DeleterType = D;

	UniquePtr() noexcept :cPair{ zero_and_variadic_arg_t{} } {}
	UniquePtr(std::nullptr_t) noexcept :cPair{ zero_and_variadic_arg_t{} } {}
	explicit UniquePtr(Pointer Ptr) noexcept :cPair{ zero_and_variadic_arg_t{}, Ptr } {}
	UniquePtr(Pointer Ptr, const DeleterType& deletor) noexcept :cPair{ one_and_variadic_arg_t{},deletor, Ptr } {}
	UniquePtr(Pointer Ptr, DeleterType&& deletor) noexcept :cPair{ one_and_variadic_arg_t {},std::move(deletor),Ptr } {}

	UniquePtr(const UniquePtr&) = delete;
	UniquePtr(UniquePtr&& Rhs) noexcept :cPair{ one_and_variadic_arg_t{},std::forward<DeleterType>(Rhs.GetDeleter()),Rhs.Release() } {}

	~UniquePtr() noexcept
	{
		if (cPair.GetSecond())
		{
			cPair.GetFirst()(cPair.GetSecond());
		}
	}

	explicit operator bool()const noexcept { return static_cast<bool>(cPair.GetSecond()); }

	// 배열버전에선 dereferencig 연산자 미제공
	// 대신 배열연산자 제공
	// ElementType& operator*() const noexcept { return *cPair.GetSecond(); }
	ElementType& operator[](std::size_t Idx)const noexcept { return cPair.GetSecond()[Idx]; }
	Pointer operator->() const noexcept { return cPair.GetSecond(); }

	UniquePtr& operator=(const UniquePtr&) = delete;
	UniquePtr& operator=(UniquePtr&& Rhs) noexcept
	{
		if (this != std::addressof(Rhs))
		{
			reset(Rhs.Release());
			cPair.GetFirst()() = std::forward<DeleterType>(Rhs.cPair.GetFirst());
		}
		return *this;
	}

	template<class T2, class D2> requires std::is_convertible_v<T2*, T*>&& std::is_convertible_v<D2, D>
	UniquePtr(UniquePtr<T2, D2>&& Rhs) noexcept :cPair{ one_and_variadic_arg_t{},std::forward<D2>(Rhs.GetDeleter()),Rhs.Release() } {}
	template<class T2, class D2> requires std::is_convertible_v<T2*, T*>&& std::is_convertible_v<D2, D>
	UniquePtr& operator=(UniquePtr<T2, D2>&& Rhs) noexcept
	{
		if (this != std::addressof(Rhs))
		{
			reset(Rhs.Release());
			cPair.GetFirst()() = std::forward<D2>(Rhs.cPair.GetFirst());
		}
		return *this;
	}

	Pointer Get()const noexcept { return cPair.GetSecond(); }
	DeleterType& GetDeleter() noexcept { return cPair.GetFirst(); }
	const DeleterType& GetDeleter() const noexcept { return cPair.GetFirst(); }

	void Swap(UniquePtr& Rhs) noexcept
	{
		std::swap(cPair.GetFirst(), Rhs.cPair.GetFirst());
		std::swap(cPair.GetSecond(), Rhs.cPair.GetSecond());
	}

	Pointer Release() noexcept { return std::exchange(cPair.GetSecond(), nullptr); }
	void Reset(Pointer Ptr = nullptr) noexcept
	{
		Pointer old{ std::exchange(cPair.GetSecond(),Ptr) };
		if (old)
			cPair.GetFirst()(old);
	}

private:
	CompressedPair<DeleterType, Pointer> cPair{};

	template<class, class>friend class UniquePtr;
};

class Animal {};
class Dog :public Animal {};
class Car {};

void ExampleMain()
{
	{
		UniquePtr<int> p1{ new int };
		UniquePtr<int> p2 = std::move(p1);
	}

	{
		auto del = [](int* Ptr) {std::free(Ptr); };
		UniquePtr<int, decltype(del)> p(static_cast<int*>(std::malloc(sizeof(int))), del);
	}

	{
		UniquePtr<Dog> d{ new Dog };
		UniquePtr<Animal> a = std::move(d);
	}

	{
		UniquePtr<int> p1{ new int };
		UniquePtr<int[]> p2{ new int[10] };

		*p1 = 10;
		p2[0] = 20;
	}
}