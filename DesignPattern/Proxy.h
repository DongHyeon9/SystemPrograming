#pragma once
#include "Headers.h"

// Proxy Pattern
// 다른 객체에 대한 접근을 제어하거나 추가적인 기능을 제공하는 대리 객체를 제공하는 디자인 패턴

// Subject 인터페이스
class Subject
{
public:
	virtual void request() abstract;
};

// RealSubject 클래스
class RealSubject : public Subject
{
public:
	void request() override
	{
		std::cout << "RealSubject의 request 호출" << std::endl;
	}
};

// Proxy 클래스
class Proxy : public Subject
{
private:
	RealSubject* realSubject{};

public:
	Proxy(RealSubject* rs) : realSubject{ rs } {}
	void request() override
	{
		std::cout << "Proxy의 request 호출" << std::endl;
		if (realSubject)
		{
			realSubject->request();
		}
	}
};

void main()
{
	RealSubject* realSubject = new RealSubject();
	Proxy* proxy = new Proxy(realSubject);
	proxy->request();
	delete proxy;
	delete realSubject;
}