#pragma once
#include "Headers.h"

// Facade Pattern
// 복잡한 서브시스템에 대한 단순화된 인터페이스를 제공하는 디자인 패턴
// 클라이언트가 서브시스템과 직접 상호작용하는 대신, Facade 객체를 통해 기능을 Wapping해서 사용

// SubsystemA 클래스
class SubsystemA
{
public:
	void operationA()
	{
		std::cout << "SubsystemA의 operationA 호출" << std::endl;
	}
};

// SubsystemB 클래스
class SubsystemB
{
public:
	void operationB()
	{
		std::cout << "SubsystemB의 operationB 호출" << std::endl;
	}
};

// Facade 클래스
class Facade
{
private:
	SubsystemA* subsystemA{};
	SubsystemB* subsystemB{};

public:
	Facade(SubsystemA* a, SubsystemB* b) : subsystemA{ a }, subsystemB{ b } {}
	void operation()
	{
		std::cout << "Facade의 operation 호출" << std::endl;
		subsystemA->operationA();
		subsystemB->operationB();
	}
};

void main()
{
	SubsystemA* subsystemA = new SubsystemA();
	SubsystemB* subsystemB = new SubsystemB();
	Facade* facade = new Facade(subsystemA, subsystemB);
	facade->operation();
	delete facade;
	delete subsystemA;
	delete subsystemB;
}