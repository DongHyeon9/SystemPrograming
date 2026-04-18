#pragma once
#include "Headers.h"

// Observer Pattern
// 객체 간의 1 : N 의존성을 정의
// 한 객체의 상태가 변경될 때 그 객체에 의존하는 모든 객체에게 알림이 가고 자동으로 업데이트되는 디자인 패턴

// Observer 인터페이스
class Observer
{
public:
	virtual ~Observer() = default;
	virtual void update() abstract;
};

// Subject 클래스
class Subject
{
private:
	std::vector<Observer*> observers{};

public:
	void attach(Observer* observer)
	{
		observers.push_back(observer);
	}
	void detach(Observer* observer)
	{
		observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
	}
	void notify()
	{
		for (Observer* observer : observers)
			observer->update();
	}
};

class ConcreteObserver : public Observer
{
public:
	void update() override
	{
		std::cout << "ConcreteObserver Update" << std::endl;
	}
};

void main()
{
	Subject* subject = new Subject();
	Observer* observer1 = new ConcreteObserver();
	Observer* observer2 = new ConcreteObserver();
	subject->attach(observer1);
	subject->attach(observer2);
	subject->notify();
	subject->detach(observer1);
	subject->notify();
	delete subject;
	delete observer1;
	delete observer2;
}