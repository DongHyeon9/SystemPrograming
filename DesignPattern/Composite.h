#pragma once
#include "Headers.h"

// Composite Pattern
// 객체를 트리 구조로 구성하여 부분-전체 계층을 표현하는 디자인 패턴

class Component
{
public:
	virtual ~Component() {}
	virtual void Operation() abstract;
};

class Leaf : public Component
{
public:
	void Operation() override
	{
		std::cout << "Leaf Operation" << std::endl;
	}
};

class Composite : public Component
{
private:
	std::vector<Component*> children{};

public:
	~Composite()
	{
		for (Component* child : children)
			delete child;
	}

	void Add(Component* component)
	{
		children.emplace_back(component);
	}

	void Operation() override
	{
		std::cout << "Composite Operation" << std::endl;
		for (Component* child : children)
			child->Operation();
	}
};

void main()
{
	Composite* root = new Composite();
	root->Add(new Leaf());
	root->Add(new Leaf());

	Composite* subComposite = new Composite();
	subComposite->Add(new Leaf());
	root->Add(subComposite);

	root->Operation();

	delete root;
}