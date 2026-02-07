#pragma once
#include "CoreMinimal.h"

template<class T = int>
class Object
{
public:
	virtual ~Object()
	{
		std::println("Destructor");
		delete[] buffer;
	}

	// 기본 생성자
	Object() = default;
	Object(std::string Name, size_t BufferSize) 
		: name{ std::move(Name) }
		, bufferSize{ BufferSize }
		, buffer{ new char[bufferSize] }
		, obj{}
	{
		std::println("Constructor");
	}

	// 복사 생성자
	Object(const Object& Rhs)
		: name{Rhs.name}
		, bufferSize{ Rhs.bufferSize }
		, buffer{ new char[bufferSize] }
		, obj{ Rhs.obj }
	{
		std::println("Copy Constructor");
		memcpy(buffer, Rhs.buffer, bufferSize);
	}

	// 이동 생성자
	Object(Object&& Rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
		: name{std::move(Rhs.name)}
		, bufferSize{ std::exchange(Rhs.bufferSize,0) }
		, buffer{ Rhs.buffer }
		, obj{ Rhs.obj }
	{
		std::println("Move Constructor");
		Rhs.buffer = nullptr;
	}

	// 복사 대입 연산자
	Object& operator=(const Object& Rhs)
	{
		std::println("Copy Assignment");

		if (this == &Rhs) return *this;

		char* newBuffer{ new char[Rhs.bufferSize] };
		memcpy(newBuffer, Rhs.buffer, Rhs.bufferSize);

		if (buffer) delete[] buffer;

		name = Rhs.name;
		bufferSize = Rhs.bufferSize;
		buffer = newBuffer;
		obj = Rhs.obj;

		return *this;
	}

	// 이동 대입 연산자
	Object& operator=(Object&& Rhs) noexcept(std::is_nothrow_move_assignable_v<T>)
	{
		std::println("Move Assignment");

		if (this == &Rhs) return *this;
		if (buffer) delete[] buffer;

		name = std::move(Rhs.name);
		bufferSize = std::exchange(Rhs.bufferSize, 0);
		buffer = Rhs.buffer;
		Rhs.buffer = nullptr;
		obj = std::move(Rhs.obj);

		return *this;
	}

	// CAS 대입 연산자
	//Object& operator=(Object Other) noexcept(std::is_nothrow_assignable_v<T>)
	//{
	//	std::println("CAS Assignment");
	//	std::swap(Other.name, name);
	//	std::swap(Other.bufferSize, bufferSize);
	//	std::swap(Other.buffer, buffer);
	//	std::swap(Other.obj, obj);
	//}

private:
	std::string name{};
	size_t bufferSize{};
	char* buffer{};
	T obj{};
};

void ExampleMain();