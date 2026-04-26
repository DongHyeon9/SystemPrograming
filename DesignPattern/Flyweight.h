#pragma once
#include "Headers.h"

// Flyweight Pattern
// 여러 객체들 간에 상태의 공통 부분들을 공유하여 사용할 수 있는 RAM에 더 많은 객체들을 포함할 수 있도록 하는 구조

// Flyweight Interface
class Flyweight
{
public:
	virtual ~Flyweight() {}
	virtual void Operation(int extrinsicState) abstract;
};

// Concrete Flyweight
class ConcreteFlyweight : public Flyweight
{
private:
	int intrinsicState{};

public:
	ConcreteFlyweight(int state) : intrinsicState{ state } {}
	void Operation(int extrinsicState) override
	{
		std::cout << "Intrinsic State: " << intrinsicState
			<< ", Extrinsic State: " << extrinsicState << std::endl;
	}
};

// Flyweight Factory
class FlyweightFactory
{
private:
	std::unordered_map<int, Flyweight*> flyweights{};

public:
	~FlyweightFactory()
	{
		for (auto& pair : flyweights)
			delete pair.second;
	}

	Flyweight* GetFlyweight(int key)
	{
		if (flyweights.find(key) == flyweights.end())
			flyweights[key] = new ConcreteFlyweight(key);
		return flyweights[key];
	}
};

void main()
{
	FlyweightFactory factory;
	Flyweight* fly1 = factory.GetFlyweight(1);
	Flyweight* fly2 = factory.GetFlyweight(2);
	Flyweight* fly3 = factory.GetFlyweight(1);
	fly1->Operation(10);
	fly2->Operation(20);
	fly3->Operation(30);
}