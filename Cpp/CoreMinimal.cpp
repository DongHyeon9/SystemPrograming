#include "CoreMinimal.h"

Benchmark::Benchmark(std::source_location Src)
	: location(Src)
{
	LogFunc_Full(std::move(Src));
}

Benchmark::~Benchmark()
{
	std::println("{0} / Time : {1}", location.function_name(), std::chrono::system_clock::now() - start);
}

void LogFunc_Full(std::source_location Src)
{
	std::println("File({0}) / Function({1}) : Line({2}) ", Src.file_name(), Src.function_name(), Src.line());
}

void LogFunc(std::source_location Src)
{
	std::println("Function({0}) : Line({1}) ", Src.function_name(), Src.line());
}