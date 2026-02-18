#include "Inheritance.h"

class Base
{
public:
	void Func1()
	{
		std::println("Base");
	}

private:
	// 1. 추상클래스여도 구현을 가질 수 있음
	virtual void Func2() abstract
	{
		std::println("Base::Func1");
	}
};

class Derived1 : private Base
{
public:
	// 2. private 상속을 받아도 자식 클래스에선 사용할 수 있음
	void Func3()
	{
		Func1();	
	}

	// 3. private으로 선언된 함수여도 상속 받을 수 있음
	// -> 접근지정자는 호출에 대한것이지 상속에 관한것이 아님
	void Func2()override 
	{
		std::println("Derived1::Func1");
	}

};

class Derived2 : public Derived1
{
private:
	int& data;
public:
	Derived2(int Data) :data{ Data } {}

	// 4. 부모클래스에서 private으로 상속받은 조상클래스의 함수는 사용할 수 없음
	void Func4()
	{
		//Func1();
	}

	// 5. 참조로 받은 값(혹은 *const)은 const 맴버함수에서도 수정가능
	// -> 객체의 내부의 값을 바꾸는게 아닌 객체 내부의 데이터가 가르키는 값을 변경하는거라 가능
	void Func5() const
	{
		data = 30;
	}
};

void ExampleMain()
{

}