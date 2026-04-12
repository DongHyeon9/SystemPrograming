#pragma once
#include "Headers.h"

// Adapter Pattern
// 인터페이스 호환이 되지 않는 클래스들을 함께 사용할 수 있도록 변환하는 디자인 패턴
// STL에선 다양한 어댑터가 존재함
// STL의 어댑터 : stack, queue, priority_queue, adapter_iterator 등

// 클래스 어댑터와 객체 어댑터가 있음
// 클래스 어댑터 : 상속을 이용하여 인터페이스를 변환하는 방식
// 객체 어댑터 : 컴포지션을 이용하여 인터페이스를 변환하는 방식

// Target 인터페이스
class Target 
{
public:
	virtual void request() abstract;
};

// Adaptee 클래스
class Adaptee 
{
public:
	void SpecificRequest() 
	{
		std::cout << "Adaptee의 SpecificRequest 호출" << std::endl;
	}
};

// Adapter 클래스
class Adapter : public Target 
{
private:
	Adaptee* adaptee{};

public:
	Adapter(Adaptee* a) : adaptee{ a } {}
	void request() override 
	{
		std::cout << "Adapter의 request 호출" << std::endl;
		adaptee->SpecificRequest();
	}
};

void main() 
{
	Adaptee* adaptee = new Adaptee();
	Target* adapter = new Adapter(adaptee);
	adapter->request();
	delete adapter;
	delete adaptee;
}