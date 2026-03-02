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
#include <mutex>
#include <random>
#include <ranges>

template<class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		static T inst{};
		return &inst;
	}

	Singleton(const Singleton<T>&) = delete;
	Singleton<T>& operator=(const Singleton<T>&) = delete;

protected:
	Singleton() = default;
};

class StatLogger : public Singleton<StatLogger>
{
public:
	void PushStat(const std::source_location& Func, double Sec, std::string Tag)
	{
		StatLoggerKey key{ Func, std::move(Tag) };

		if (stats.find(key) == stats.end())
		{
			std::lock_guard<std::mutex> statLock{ mtx };
			auto& val{ stats[key] };
			val.avg = Sec;
			val.max = Sec;
			val.min = Sec;
			val.callCount = 1;
		}
		else
		{
			auto& val{ stats[key] };
			std::lock_guard<std::mutex> lock{ val.mtx };
			double total = val.avg * val.callCount + Sec;
			val.avg = total / (val.callCount + 1);

			val.min = std::min(val.min, Sec);
			val.max = std::max(val.max, Sec);
			++val.callCount;
		}
	}

	void Log()
	{
		system("cls");
		std::cout << std::fixed << std::setprecision(3);
		std::lock_guard<std::mutex> statLock{ mtx };
		for (auto& stat : stats)
		{
			std::lock_guard<std::mutex> lock{ stat.second.mtx };
			std::cout << stat.first.src.function_name() << " / " << stat.first.tag << std::endl;

			std::cout << "CallCount " << stat.second.callCount
				<< " / Avg " << stat.second.avg
				<< " / Min " << stat.second.min
				<< " / Max " << stat.second.max
				<< std::endl;
		}
	}

private:
	struct StatLoggerValue
	{
		StatLoggerValue() = default;
		StatLoggerValue(const StatLoggerValue&) = delete;
		StatLoggerValue& operator=(const StatLoggerValue&) = delete;

		StatLoggerValue(StatLoggerValue&&) noexcept = default;
		StatLoggerValue& operator=(StatLoggerValue&&) noexcept = default;

		std::size_t callCount{};
		double avg{};
		double min{};
		double max{};
		std::mutex mtx{};
	};

	struct StatLoggerKey
	{
		StatLoggerKey(const std::source_location& s, std::string t) : src(s), tag(std::move(t)) {}
		std::source_location src{};
		std::string tag{};

		bool operator==(const StatLoggerKey& rhs) const
		{
			return src.file_name() == rhs.src.file_name()
				&& src.function_name() == rhs.src.function_name()
				&& tag == rhs.tag;
		}
	};

	struct StatLoggerKeyHash
	{
		std::size_t operator()(const StatLoggerKey& key) const noexcept
		{
			std::size_t h1 = std::hash<std::string_view>{}(key.src.file_name());
			std::size_t h2 = std::hash<std::string_view>{}(key.src.function_name());
			std::size_t h3 = std::hash<std::string>{}(key.tag);

			return ((h1 * 31 + h2) * 31) + h3;
		}
	};

	std::unordered_map<StatLoggerKey, StatLoggerValue, StatLoggerKeyHash> stats{};
	std::mutex mtx{};
};

class Benchmark
{
public:
	Benchmark(std::string Tag = { "All" }, std::source_location Src = std::source_location::current()) :tag{ std::move(Tag) }, location{ std::move(Src) } {}
	~Benchmark() { StatLogger::GetInstance()->PushStat(location, std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count(), tag); }

private:
	std::chrono::high_resolution_clock::time_point start{ std::chrono::high_resolution_clock::now() };
	std::source_location location{};
	std::string tag{};
};

class Stopwatch
{
public:
	Stopwatch(){}
	~Stopwatch() { std::cout << (std::chrono::high_resolution_clock::now() - start).count() << std::endl; }

private:
	std::chrono::high_resolution_clock::time_point start{ std::chrono::high_resolution_clock::now() };
};

template<class F,class ...Ts>
decltype(auto) Chronometry(F&& Func,Ts&&...Args)
{
	Stopwatch sw{};
	return std::invoke(std::forward<F>(Func), std::forward<Ts>(Args)...);
}

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
