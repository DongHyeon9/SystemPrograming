#pragma once
#include "Headers.h"

// Memento Pattern
// 해당 객체의 이전 상태를 저장하고 복원할 수 있게 해주는 행동 디자인 패턴

// Memento 클래스
class Memento
{
private:
	int state{};

public:
	Memento(int s) : state{ s } {}
	int GetState() const { return state; }
};

// Originator 클래스
class Originator
{
private:
	int state{};

public:
	void SetState(int s) { state = s; }
	int GetState() const { return state; }
	Memento* CreateMemento() { return new Memento(state); }
	void RestoreMemento(Memento* m) { if (m) state = m->GetState(); }
};

// Caretaker 클래스
class Caretaker
{
private:
	std::vector<Memento*> mementos{};

public:
	~Caretaker()
	{
		for (Memento* m : mementos)
			delete m;
	}
	void AddMemento(Memento* m) { mementos.emplace_back(m); }
	Memento* GetMemento(int index) { return (index >= 0 && index < mementos.size()) ? mementos[index] : nullptr; }
};

void main()
{
	Originator originator;
	Caretaker caretaker;
	originator.SetState(1);
	caretaker.AddMemento(originator.CreateMemento());
	originator.SetState(2);
	caretaker.AddMemento(originator.CreateMemento());
	originator.SetState(3);
	caretaker.AddMemento(originator.CreateMemento());
	std::cout << "Current State: " << originator.GetState() << std::endl;
	originator.RestoreMemento(caretaker.GetMemento(0));
	std::cout << "Restored State: " << originator.GetState() << std::endl;
	originator.RestoreMemento(caretaker.GetMemento(1));
	std::cout << "Restored State: " << originator.GetState() << std::endl;
	originator.RestoreMemento(caretaker.GetMemento(2));
	std::cout << "Restored State: " << originator.GetState() << std::endl;
}