#pragma once
#include "Headers.h"

// Bridge Pattern
// 구현부에서 추상층을 분리하여 독립적으로 변형할 수 있도록 하는 디자인 패턴
// C++에선 Pimpl Idiom이 Bridge Pattern의 한 형태로 볼 수 있음
// 컴파일 시간을 줄이는 효과가 있음

// Implementor 인터페이스
class Implementor
{
public:
	virtual void operationImpl() abstract;
};

// ConcreteImplementor 클래스
class ConcreteImplementor : public Implementor
{
public:
	void operationImpl() override
	{
		std::cout << "ConcreteImplementor의 operationImpl 호출" << std::endl;
	}
};

// Abstraction 클래스
class Abstraction
{
private:
	Implementor* implementor{};

public:
	Abstraction(Implementor* impl) : implementor{ impl } {}
	void operation()
	{
		std::cout << "Abstraction의 operation 호출" << std::endl;
		if (implementor)
		{
			implementor->operationImpl();
		}
	}
};

void main()
{
	Implementor* implementor = new ConcreteImplementor();
	Abstraction* abstraction = new Abstraction(implementor);
	abstraction->operation();
	delete abstraction;
	delete implementor;
}