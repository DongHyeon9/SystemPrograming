#pragma once
#include "Headers.h"

// Prototype Pattern
// 코드를 그들의 클래스들에 의존시키지 않고 기존 객체들을 복사할 수 있도록 하는 생성 디자인 패턴

// Prototype Interface
class Prototype
{
public:
	virtual ~Prototype() {}
	virtual Prototype* Clone() const abstract;
};

// Concrete Prototype
class ConcretePrototype : public Prototype
{
private:
	int data{};

public:
	ConcretePrototype(int d) : data{ d } {}
	Prototype* Clone() const override
	{
		return new ConcretePrototype(*this);
	}
	void Display() const
	{
		std::cout << "ConcretePrototype with data: " << data << std::endl;
	}
};

void main()
{
	ConcretePrototype* original = new ConcretePrototype(42);
	ConcretePrototype* copy = static_cast<ConcretePrototype*>(original->Clone());
	original->Display();
	copy->Display();
	delete original;
	delete copy;
}