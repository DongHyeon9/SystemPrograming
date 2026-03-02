#include "BenchmarkSample.h"

void WaitRandom(int MaxTime)
{
	static std::mt19937_64 rng{ std::random_device{}() };
	std::this_thread::sleep_for(std::chrono::milliseconds{ std::uniform_int_distribution{ 0, MaxTime }(rng) });
}

void Func1() { while (true) { Benchmark benchMark{}; WaitRandom(1000); } }
void Func2() { while (true) { Benchmark benchMark{}; WaitRandom(2000); } }
void Func3() { while (true) { Benchmark benchMark{}; WaitRandom(3000); } }
void Func4() { while (true) { Benchmark benchMark{}; WaitRandom(4000); } }
void Func5() { while (true) { Benchmark benchMark{}; WaitRandom(5000); } }

void ExampleMain()
{
	std::thread t1{ Func1 };
	std::thread t2{ Func2 };
	std::thread t3{ Func3 };
	std::thread t4{ Func4 };
	std::thread t5{ Func5 };

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds{ 5 });
		StatLogger::GetInstance()->Log();
	}
}