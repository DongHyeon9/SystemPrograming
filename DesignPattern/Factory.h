#pragma once
#include "Headers.h"

// Factory Pattern
// 부모 클래스에서 객체들을 생성할 수 있는 인터페이스를 제공
// -> 자식 클래스들이 생성될 객체들의 유형을 변경할 수 있도록 하는 생성 패턴

// Product Interface
class Product
{
public:
	virtual ~Product() {}
	virtual void Use() abstract;
};

// Concrete Product A
class ConcreteProductA : public Product
{
public:
	void Use() override
	{
		std::cout << "Using ConcreteProductA" << std::endl;
	}
};

// Concrete Product B
class ConcreteProductB : public Product
{
public:
	void Use() override
	{
		std::cout << "Using ConcreteProductB" << std::endl;
	}
};

// Factory Interface
class Factory
{
public:
	virtual ~Factory() {}
	virtual Product* CreateProduct() abstract;
};

// Concrete Factory A
class ConcreteFactoryA : public Factory
{
public:
	Product* CreateProduct() override
	{
		return new ConcreteProductA();
	}
};

// Concrete Factory B
class ConcreteFactoryB : public Factory
{
public:
	Product* CreateProduct() override
	{
		return new ConcreteProductB();
	}
};

void main()
{
	Factory* factoryA = new ConcreteFactoryA();
	Product* productA = factoryA->CreateProduct();
	productA->Use();
	Factory* factoryB = new ConcreteFactoryB();
	Product* productB = factoryB->CreateProduct();
	productB->Use();
	delete productA;
	delete factoryA;
	delete productB;
	delete factoryB;
}