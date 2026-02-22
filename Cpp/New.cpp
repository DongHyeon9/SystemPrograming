#include "New.h"

class Foo
{
public:
	Foo() { std::cout << "Foo Constructor" << std::endl; }
	~Foo() { std::cout << "Foo Destructor" << std::endl; }
private:
	int data{};
};

// operator new
void* operator new(std::size_t Size)
{
	void* ptr = std::malloc(Size);
	if (ptr == nullptr)
		throw std::bad_alloc();
	std::println("new size : {0} / addr : {1}", Size, ptr);
	return ptr;
}

// operator new[]
void* operator new[](std::size_t Size)
{
	void* ptr = std::malloc(Size);
	if (ptr == nullptr)
		throw std::bad_alloc();
	std::println("new[] size : {0} / addr : {1}", Size, ptr);
	return ptr;
}

// placement new
void* operator new(std::size_t Size, const char* Tag)
{
	void* ptr = std::malloc(Size);
	if (ptr == nullptr)
		throw std::bad_alloc();
	std::println("placement new size : {0} / addr : {1} / Tag : {2}", Size, ptr, Tag);
	return ptr;
}

// placement new[]
void* operator new[](std::size_t Size, const char* Tag)
{
	void* ptr = std::malloc(Size);
	if (ptr == nullptr)
		throw std::bad_alloc();
	std::println("placement new[] size : {0} / addr : {1} / Tag : {2}", Size, ptr, Tag);
	return ptr;
}

// operator delete
void operator delete(void* Ptr) noexcept
{
	std::println("delete addr : {}", Ptr);
	std::free(Ptr);
}

// operator delete[]
void operator delete[](void* Ptr) noexcept
{
	std::println("delete[] addr : {}", Ptr);
	std::free(Ptr);
}

// placement delete
void operator delete(void* Ptr, const char* Tag) noexcept
{
	std::println("placement delete addr : {0} / Tag : {1}", Ptr, Tag);
	std::free(Ptr);
}

// Placement Delete는 직접 호출하는게 아닌, Placement new가 예외를 던질경우 호출되는것
// 그래서 placement delete[]를 직접 호출하게되면 배열의 메타정보에 관한것이 제대로 인식되지않아 프로그램이 종료된다
// placement delete[]
void operator delete[](void* Ptr, const char* Tag) noexcept
{
	std::println("placement delete addr : {0} / Tag : {1}", Ptr, Tag);
	std::free(Ptr);
}

void ExampleMain()
{
	{
		std::cout << "=====operator new=====" << std::endl;
		Foo* data = new Foo;
		std::cout << "=====operator delete=====" << std::endl;
		delete data;
	}

	{
		// 배열 동적 할당 시 배열에 대한 metaData를 저장하기 때문에
		// 실제 사이즈보다 큰 메모리가 할당 됨
		// https://en.cppreference.com/w/cpp/language/new.html
		// Allocation 섹션 참고
		std::cout << "=====operator new[]=====" << std::endl;
		Foo* data = new Foo[5];
		std::cout << "=====operator delete[]=====" << std::endl;
		delete[] data;
	}

	{
		std::cout << "=====placement new=====" << std::endl;
		Foo* data = new("Placement New Tag") Foo;
		std::cout << "=====placement delete=====" << std::endl;
		// 그냥 delete를 쓰면 void operator delete(void* Ptr)이 호출
		data->~Foo();
		operator delete(data, "Placement Delete Tag");
	}

	{
		std::cout << "=====placement new[]=====" << std::endl;
		Foo* data = new("Placement New[] Tag") Foo[5];
		std::cout << "=====placement delete[]=====" << std::endl;
		delete[] data;
		/* 
		이렇게 호출하게되면 error
		for (int i = 0; i < 5; ++i)
			data[i].~Foo();
		operator delete[](data, "Placement Delete[] Tag");
		*/
	}
}