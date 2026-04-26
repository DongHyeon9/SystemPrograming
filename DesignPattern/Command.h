#pragma once
#include "Headers.h"

// Command Pattern
// 요청을 요청에 대한 모든 정보가 포함된 독립실행형 객체로 변환하는 행동 디자인 패턴

// Command Interface
class Command
{
public:
	virtual ~Command() {}
	virtual void Execute() abstract;
};

// Receiver
class Receiver
{
public:
	void Action()
	{
		std::cout << "Receiver Action Executed" << std::endl;
	}
};

// Concrete Command
class ConcreteCommand : public Command
{
private:
	Receiver* receiver{};

public:
	ConcreteCommand(Receiver* r) : receiver{ r } {}
	void Execute() override
	{
		if (receiver)
			receiver->Action();
	}
};

// Invoker
class Invoker
{
private:
	Command* command{};

public:
	void SetCommand(Command* cmd)
	{
		command = cmd;
	}
	void Invoke()
	{
		if (command)
			command->Execute();
	}
};

void main()
{
	Receiver receiver;
	ConcreteCommand command(&receiver);
	Invoker invoker;
	invoker.SetCommand(&command);
	invoker.Invoke();
}