#include <iostream>
#include <Windows.h>
#include <DbgHelp.h>
#include <tchar.h>
#pragma comment(lib, "DbgHelp.lib")

#ifdef _WIN64
using ADDR = ULONGLONG;
#elif
using ADDR = DWORD;
#endif

const char* TARGET_MODULE_NAME{ "user32.dll" };

const char* TARGET_API_NAME
{
#ifdef UNICODE
	"MessageBoxW"
#else
	"MessageBoxA"
#endif
};
using TARGET_API = UINT(__stdcall*)(HWND, TCHAR*, TCHAR*, UINT);

UINT __stdcall HookFunc(HWND hWnd, TCHAR* Str1, TCHAR* Str2, UINT Btn)
{
	::_tprintf(_T("API Hook : %s, %s\n"), Str1, Str2);
	HMODULE hDll{ ::GetModuleHandleA(TARGET_MODULE_NAME) };
	TARGET_API func{ reinterpret_cast<TARGET_API>(::GetProcAddress(hDll, TARGET_API_NAME)) };

	// 타이틀이랑 텍스트 반대로 출력
	return func(hWnd, Str2, Str1, Btn);
}

void APIHook(HMODULE hExe, const char* dllname, void* api, void* func)
{
	// 1. PE에서 import 섹션의 주소를 얻어낸다
	ULONG sz{};
	IMAGE_IMPORT_DESCRIPTOR* pImage{ static_cast<IMAGE_IMPORT_DESCRIPTOR*>(::ImageDirectoryEntryToData(hExe,TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT,&sz)) };
	::_tprintf(_T("Address Import Directory : %p\n"), pImage);

	char* moduleAddr{ reinterpret_cast<char*>(hExe) };

	// 2. 해당 DLL의 정보를 가진 항목을 찾아낸다
	for (; pImage->Name; ++pImage)
	{
		char* s{ reinterpret_cast<char*>(hExe) + pImage->Name };
		if (::_strcmpi(s, dllname) == 0)break;
	}
	if (pImage->Name == 0)
	{
		::printf("Can Not Found %s\n", dllname);
		return;
	}
	::printf("%s import table : %p\n", dllname, pImage);

	// 3. 함수주소를 담은 table을 조사한다
	IMAGE_THUNK_DATA* pThunk{ reinterpret_cast<IMAGE_THUNK_DATA*>(moduleAddr + pImage->FirstThunk) };
	for (; pThunk->u1.Function; ++pThunk)
	{
		if (pThunk->u1.Function == reinterpret_cast<ADDR>(api))
		{
			ADDR* addr = &(pThunk->u1.Function);

			DWORD old{};
			VirtualProtect(addr, sizeof(ADDR), PAGE_READWRITE, &old);

			*addr = reinterpret_cast<ADDR>(func);

			VirtualProtect(addr, sizeof(ADDR), old, &old);
			break;
		}
	}
}

int main()
{
	APIHook(GetModuleHandle(0), TARGET_MODULE_NAME, &MessageBox, &HookFunc);
	MessageBox(0, _T("API Hooking"), _T("Hooking!!"), 0);
}