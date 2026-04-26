#pragma once
#include "Headers.h"

// State Pattern
// 객체의 내부 상태가 변경될 때 해당 객체가 그의 행동을 변경할 수 있도록 하는 행동 디자인 패턴

// State 인터페이스
class State
{
public:
	virtual ~State() {}
	virtual void Handle() abstract;
};

// Concrete State A
class ConcreteStateA : public State
{
public:
	void Handle() override
	{
		std::cout << "Handling state A" << std::endl;
	}
};

// Concrete State B
class ConcreteStateB : public State
{
public:
	void Handle() override
	{
		std::cout << "Handling state B" << std::endl;
	}
};

// Context 클래스
class Context
{
private:
	State* state{};

public:
	Context(State* s) : state{ s } {}
	~Context() { delete state; }
	void SetState(State* s)
	{
		if (state)
			delete state;
		state = s;
	}
	void Request()
	{
		if (state)
			state->Handle();
	}
};

void main()
{
	Context context(new ConcreteStateA());
	context.Request();
	context.SetState(new ConcreteStateB());
	context.Request();
}