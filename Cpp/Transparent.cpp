#include "Transparent.h"

struct People
{
	std::string name{};
	int age{};
	People(std::string Name, int Age) :name{ std::move(Name) }, age{ Age } {}
};

struct PeopleCompare
{
	bool operator()(const People& Lhs, const People& Rhs) const
	{
		return Lhs.name < Rhs.name;
	}
	bool operator()(const People& Lhs, std::string_view Rhs)const
	{
		return Lhs.name < Rhs;
	}
	bool operator()(std::string_view Lhs, const People& Rhs)const
	{
		return Lhs < Rhs.name;
	}

	// C++ 14부터 추가
	// 비교 객체 안에 is_transparent 타입을 정의해 놓으면 한가지 맴버에 대해서 비교할 수 있다
	using is_transparent = void;
};

void ExampleMain()
{
	std::set<People, PeopleCompare> s{};

	s.emplace("Kim", 20);
	s.emplace("Lee", 25);
	s.emplace("Park", 40);
	s.emplace("Choi", 30);

	auto p = s.find("Kim");

	std::cout << p->name << ", " << p->age << std::endl;
}