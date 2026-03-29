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

	// C++ 14부터 추가 (SFINAE)
	// 비교 객체 안에 is_transparent 타입을 정의해 놓으면 한가지 맴버에 대해서 비교할 수 있다
	// template<typename Key, typename Pred = std::less<Key>, typename Alloc = std::allocator<Key>> class set 은
	// iterator find(const Key& key)로 정의되어있지만
	// template<typename Other, typename = Pred::is_transparent> iterator find(const Other& key)로도 정의되어 있다
	using is_transparent = void;

	// SFINAE
	// Substitution Failure Is Not An Error
	// 치환 실패는 에러가 아니다
	// 이 템플릿 안 되네? → 에러 말고 다른 함수 써볼게
	// 조건에 맞는 함수만 선택하기 위함

	// C++ 14부턴
	// std::less<void>를 사용하면 다른 타입에 대해서 비교가 가능하다
	// ->std::less<void>는 operator()가 템플릿으로 특수화 되어있다
	// std::set<People, std::less<void>>이렇게 쓰면
	// bool operator()(const People& Lhs, std::string_view Rhs)const
	// bool operator()(std::string_view Lhs, const People& Rhs)const
	// 이 두개를 전역으로 비교함수를 만들었을 때 비교가 가능하다
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