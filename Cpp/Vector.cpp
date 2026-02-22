#include "Vector.h"

// Policy Base Design (단위 전략 디자인)
// 클래스가 사용하는 정책을 템플릿 인자로 전달 받아서 교체 가능하게 만드는 디자인 기술
template<class T>
struct Allocator
{
	T* allocate(std::size_t N)
	{
		T* p{ static_cast<T*>(std::malloc(N * sizeof(T))) };
		std::cout << "allocate" << std::endl;
		return p;
	}
	void deallocate(T* Ptr, std::size_t N)noexcept
	{
		std::free(Ptr);
		std::cout << "deallocate" << std::endl;
	}
	// allocator_traits를 사용하고 있기 때문에
	// construct와 destroy는 구현하지 않으면 기본으로 제공하는 생성자와 소멸자호출 문법을 사용하게 된다
	template<class ... ARGS>
	void construct(T* Ptr, ARGS&&...Args)
	{
		new(Ptr) T(std::forward<ARGS>(Args)...);
		std::cout << "Allocator construct" << std::endl;
	}
	void destroy(T* Ptr)
	{
		Ptr->~T();
		std::cout << "Allocator destroy" << std::endl;
	}

	using value_type = T;

	template<class U>
	Allocator(const Allocator<U>&)noexcept {}

	Allocator() = default;

	template<class U>
	struct Rebind
	{
		using Other = Allocator<U>;
		// 쓸 때(C++98)
		// typename T::template Rebind<int>::Other allocator{};
		// 의 형태로 사용가능하다
		// template = Rebind가 템플릿이라는걸 명시적으로 알려줌

		// C++11
		// typename std::allocator_traits<T>::template rebind_alloc<int> allocator{};
		// allocator_traits기 때문에 없으면 기본 rebind 사용

		// C++20 부턴 std::allocator<T> 가 rebind 맴버를 제공하지 않음
		// 그래서 C++11스타일을 사용 권장
	};
};
// ==, !=연산자
// 현재 Allocator의 맴버 데이터가 없음
template<class T, class U>
bool operator==(const Allocator<T>&, const Allocator<U>&) { return true; }

template<class T, class U>
bool operator!=(const Allocator<T>&, const Allocator<U>&) { return false; }

template<class T, class Alloc = std::allocator<T>>
class Vector
{
public:
	Vector(std::size_t Size, const T& Value) :size{ Size }, capacity{ Size }
	{
		// 메모리만 할당
		//buffer = static_cast<T*>(operator new(sizeof(T) * Size));
		buffer = std::allocator_traits<Alloc>::allocate(alloc, sizeof(T) * Size);

		std::size_t i{};
		try
		{
			for (i = 0; i < Size; ++i)
			{
				// 전통 placement new
				// new(&buffer[i]) T(Value);

				// c++ 20 placement new 함수
				// std::construct_at(&buffer[i], Value);

				std::allocator_traits<Alloc>::construct(alloc, &buffer[i], Value);
			}
		}
		catch (...)
		{
			int j = static_cast<int>(i) - 1;
			for (; 0 <= j; --j)
			{
				// 전통 소멸자 호출
				//buffer[j].~T();

				// c++ 17 소멸자 호출
				//std::destroy_at(&buffer[j]);

				std::allocator_traits<Alloc>::destroy(alloc, &buffer[j]);
			}

			//operator delete(buffer);
			std::allocator_traits<Alloc>::deallocate(alloc, buffer, capacity);
			size = 0;
			capacity = 0;

			throw;
		}
	}
	~Vector()
	{
		int i = static_cast<int>(size) - 1;
		for (; 0 <= i; --i)
		{
			// 전통 소멸자 호출
			//buffer[i].~T();

			// c++ 17 소멸자 호출
			//std::destroy_at(&buffer[i]);

			std::allocator_traits<Alloc>::destroy(alloc, &buffer[i]);
		}

		//operator delete(buffer);
		std::allocator_traits<Alloc>::deallocate(alloc, buffer, capacity);
	}

	T& operator[](std::size_t Idx) { return buffer[Idx]; }
private:
	T* buffer{};
	std::size_t size{};
	std::size_t capacity{};

	Alloc alloc{};
};

// bool 부분 특수화
template<class Alloc>
class Vector<bool, Alloc>
{
public:
	using BYTE = unsigned char;
	using RebindAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<BYTE>;

	// Temporary Proxy
	struct BitProxy
	{

		BYTE* buffer{};
		std::size_t idx{};
		BitProxy(BYTE* Buffer, std::size_t Idx) :buffer{ Buffer }, idx{ Idx } {}
		BitProxy& operator=(bool Value)
		{
			BYTE& byte = buffer[idx >> 3];					// idx / 8
			BYTE bit = static_cast<BYTE>(1u << (idx & 7));	// idx % 8
			byte = Value ? (byte | bit) : (byte & ~bit);
			return *this;
		}
		operator bool() const noexcept
		{
			return (buffer[idx >> 3] >> (idx & 7)) & 1u;
		}
	};

public:
	Vector(std::size_t Size, bool Value = false) :size{ Size }, capacity{ (Size + 7) >> 3 }
	{
		buffer = std::allocator_traits<RebindAlloc>::allocate(alloc, capacity);
		// 비트 초기화
		std::memset(buffer, Value ? 0xFF : 0x00, capacity);
		if (Size & 7)
			buffer[capacity - 1] &= static_cast<BYTE>((1u << (Size % 8)) - 1);
	}
	~Vector()
	{
		std::allocator_traits<RebindAlloc>::deallocate(alloc, buffer, capacity);
	}

	BitProxy operator[](std::size_t Idx) { return BitProxy{ buffer, Idx }; }
private:

	BYTE* buffer{};
	std::size_t size{};
	std::size_t capacity{};

	RebindAlloc alloc{};
};

class Foo
{
public:
	Foo() { std::cout << "Foo()" << std::endl; }
	Foo(int) { std::cout << "Foo(int)" << std::endl; }
	Foo(int, int) { std::cout << "Foo(int, int)" << std::endl; }
	Foo(const Foo&) { std::cout << "Foo(const Foo&)" << std::endl; }
	~Foo() { std::cout << "~Foo()" << std::endl; }
};

void ExampleMain()
{
	{
		std::cout << "=====Allocator=====" << std::endl;
		Allocator<Foo> alloc{};
		//Foo* f = alloc.allocate(1);
		Foo* f = std::allocator_traits<Allocator<Foo>>::allocate(alloc, 1);

		//alloc.construct(f, 1, 2);
		std::allocator_traits<Allocator<Foo>>::construct(alloc, f, 1, 2);
		//alloc.destroy(f);
		std::allocator_traits<Allocator<Foo>>::destroy(alloc, f);

		//alloc.deallocate(f, 1);
		std::allocator_traits<Allocator<Foo>>::deallocate(alloc, f, 1);
	}

	{
		std::cout << "=====Vector=====" << std::endl;
		Foo f{};
		Vector<Foo, Allocator<Foo>> v(5, f);
	}

	{
		std::cout << "=====Vector Bool=====" << std::endl;
		Vector<bool, Allocator<Foo>> v(10, false);
		v[0] = true;
		if (v[0])
			std::cout << "Suc" << std::endl;
		v[9] = true;
		if (v[9])
			std::cout << "Suc" << std::endl;
	}
}