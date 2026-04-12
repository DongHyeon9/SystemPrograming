#pragma once
#include "Headers.h"

// Decorator Pattern
// 객체에 추가적인 기능을 동적으로 부여하는 디자인 패턴

class Component 
{
public:
	virtual void Operation() abstract;
};

class ConcreteComponent : public Component 
{
public:
	void Operation() override 
	{
		std::cout << "ConcreteComponent Operation" << std::endl;
	}
};

class Decorator : public Component 
{
protected:
	Component* component{};

public:
	Decorator(Component* comp) : component(comp) {}
	void Operation() override 
	{
		if (component) {
			component->Operation();
		}
	}
};

class ConcreteDecoratorA : public Decorator {
public:
	ConcreteDecoratorA(Component* comp) : Decorator(comp) {}
	
	void Operation() override 
	{
		Decorator::Operation();
		std::cout << "ConcreteDecoratorA Operation" << std::endl;
	}
};

class ConcreteDecoratorB : public Decorator 
{
public:
	ConcreteDecoratorB(Component* comp) : Decorator(comp) {}

	void Operation() override 
	{
		Decorator::Operation();
		std::cout << "ConcreteDecoratorB Operation" << std::endl;
	}
};

void main() {
	Component* component = new ConcreteComponent();
	Component* decoratorA = new ConcreteDecoratorA(component);
	Component* decoratorB = new ConcreteDecoratorB(decoratorA);
	decoratorB->Operation();
	delete decoratorB;
	delete decoratorA;
	delete component;
}