#include "RuleOfFive.h"

void ExampleMain()
{
	Object obj1{ "Obj", 10 };
	Object obj2 = obj1;

	Object obj3 = std::move(obj1);
	Object obj4 = std::move_if_noexcept(obj2);
}