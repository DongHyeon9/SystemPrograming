#pragma once
#include "Headers.h"

// Strategy Pattern
// 객체가 할 수 있는 행위를 캡슐화하여 필요에 따라 교체할 수 있게 만든다

#include <iostream>
#include <string>

class Condition 
{
public:
	std::string GetCondition()const { return condition; }

protected:
	Condition(std::string Condition) : condition(Condition) {}

protected:
	std::string condition;

};

class Targeting : public Condition {
public:
	Targeting() :Condition("Targeting") {}
};

class NonTargeting : public Condition {
public:
	NonTargeting() :Condition("NonTargeting") {}
};

class Area : public Condition {
public:
	Area() :Condition("Area") {}
};

class Skill
{
private:
	Condition* condition;

public:
	~Skill() {
		if (condition)
			delete condition;
	}
	void SetCondition(Condition* NewCondition) {
		if (condition)
			delete condition;
		condition = NewCondition;
	}
	void CheckCondition() {
		if (condition)
			std::cout << condition->GetCondition() << std::endl;
		else
			throw;
	}
};