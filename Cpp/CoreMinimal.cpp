#include "CoreMinimal.h"

void LogFunc_Full(std::source_location Src)
{
	std::println("File({0}) / Function({1}) : Line({2}) ", Src.file_name(), Src.function_name(), Src.line());
}

void LogFunc(std::source_location Src)
{
	std::println("Function({0}) : Line({1}) ", Src.function_name(), Src.line());
}