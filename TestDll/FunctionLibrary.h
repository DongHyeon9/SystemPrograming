#pragma once
#include <Windows.h>

void OnProcessAttach(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
void OnProcessDetach(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
void OnThreadAttach(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
void OnThreadDetach(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);