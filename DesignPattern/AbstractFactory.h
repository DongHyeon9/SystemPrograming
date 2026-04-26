#pragma once
#include "Headers.h"

// Abstract Factory Pattern
// 관련 객체들의 구상 클래스들을 지정하지 않고도 관련 객체들의 모음을 생성할 수 있도록 하는 생성패턴

// Abstract Product
class AbstractProductA 
{
public:
	virtual ~AbstractProductA() {}
	virtual void Use() abstract;
};

// Concrete Product A1
class ConcreteProductA1 : public AbstractProductA 
{
public:
	void Use() override 
	{
		std::cout << "Using ConcreteProductA1" << std::endl;
	}
};

// Concrete Product A2
class ConcreteProductA2 : public AbstractProductA 
{
public:
	void Use() override 
	{
		std::cout << "Using ConcreteProductA2" << std::endl;
	}
};

// Abstract Factory
class AbstractFactory 
{
public:
	virtual ~AbstractFactory() {}
	virtual AbstractProductA* CreateProductA() abstract;
};

// Concrete Factory 1
class ConcreteFactory1 : public AbstractFactory 
{
public:
	AbstractProductA* CreateProductA() override 
	{
		return new ConcreteProductA1();
	}
};

// Concrete Factory 2
class ConcreteFactory2 : public AbstractFactory 
{
public:
	AbstractProductA* CreateProductA() override 
	{
		return new ConcreteProductA2();
	}
};

void main() {
	AbstractFactory* factory1 = new ConcreteFactory1();
	AbstractProductA* productA1 = factory1->CreateProductA();
	productA1->Use();
	AbstractFactory* factory2 = new ConcreteFactory2();
	AbstractProductA* productA2 = factory2->CreateProductA();
	productA2->Use();
	delete productA1;
	delete productA2;
	delete factory1;
	delete factory2;
}