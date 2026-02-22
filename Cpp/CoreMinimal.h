// C++ 23
// msvc 143
// visual studio 2022

#pragma once
#include <chrono>
#include <thread>
#include <iostream>
#include <functional>
#include <source_location>
#include <print>
#include <algorithm>
#include <type_traits>
#include <vector>
#include <atomic>
#include <string_view>
#include <set>

class Benchmark
{
public:
	Benchmark(std::source_location Src = std::source_location::current());
	~Benchmark();

private:
	std::chrono::system_clock::time_point start{ std::chrono::system_clock::now() };
	std::source_location location{};
};

// 로깅을 위한 스트링 클래스
struct String : public std::string
{
	//기본 생성자 가져오기
	using std::string::basic_string;

	String(const String& Str) : std::string(static_cast<const std::string&>(Str))
	{
		std::println("Copy Con");
	}

	String(String&& Str) noexcept : std::string(static_cast<std::string&&>(Str))
	{
		std::println("Move Con");
	}
		
	String& operator=(const String& Str)
	{
		std::string::operator=(static_cast<const std::string&>(Str));
		std::println("Copy Ass");
		return *this;
	}

	String& operator=(String&& Str) noexcept
	{
		std::string::operator=(static_cast<const std::string&&>(Str));
		std::println("Move Ass");
		return *this;
	}
};

void LogFunc_Full(std::source_location Src = std::source_location::current());
void LogFunc(std::source_location Src = std::source_location::current());
