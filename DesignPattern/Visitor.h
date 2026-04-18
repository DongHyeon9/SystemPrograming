#pragma once
#include "Headers.h"

// Visitor Pattern
// 객체 구조에서 각 요소에 대한 연산을 정의하는 디자인 패턴

// Visitor 인터페이스
class Visitor
{
public:
	virtual void Visit(class ElementA* element) abstract;
	virtual void Visit(class ElementB* element) abstract;
};

// Element 인터페이스
class Element
{
public:
	virtual void Accept(Visitor* visitor) abstract;
};

// ConcreteElementA 클래스
class ElementA : public Element
{
public:
	void Accept(Visitor* visitor) override
	{
		visitor->Visit(this);
	}
};

// ConcreteElementB 클래스
class ElementB : public Element
{
public:
	void Accept(Visitor* visitor) override
	{
		visitor->Visit(this);
	}
};

// ConcreteVisitor 클래스
class ConcreteVisitor : public Visitor
{
public:
	void Visit(ElementA* element) override
	{
		// ElementA에 대한 연산 구현
	}

	void Visit(ElementB* element) override
	{
		// ElementB에 대한 연산 구현
	}
};

void main()
{
	Element* elementA = new ElementA();
	Element* elementB = new ElementB();
	Visitor* visitor = new ConcreteVisitor();
	elementA->Accept(visitor);
	elementB->Accept(visitor);
	delete elementA;
	delete elementB;
	delete visitor;
}