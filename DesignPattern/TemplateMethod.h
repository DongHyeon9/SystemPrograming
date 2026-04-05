#pragma once
#include "Headers.h"

// Template Method Pattern
// 알고리즘의 구조를 고정하고, 일부 단계만 바꿀 수 있도록 하는 디자인 패턴

class Base
{
public:
	void Foo()
	{
		// 고정된 전처리 작업

		Foo_impl();

		// 고정된 후처리 작업
	}

protected:
	virtual void Foo_impl() abstract;
};

class Derived1 : public Base
{
protected:
	void Foo_impl() override
	{
		// Derived1 클래스에서 구현된 Foo_impl
	}
};