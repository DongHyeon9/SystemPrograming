#include <iostream>
#include <tchar.h>
#include <Windows.h>
#include <Psapi.h>
#include <vector>
#include <array>

#include <shobjidl.h>
#include <atlbase.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>

struct ComInit
{
	ComInit() { CoInitialize(nullptr); }
	~ComInit() { CoUninitialize(); }
};

constexpr size_t PROCESS_CNT{ 1024 };

int main()
{
	// 1. 프로세스 핸들 얻기
	// 
	// 1-1. Window 이름으로 핸들 얻기
	//HWND hWnd{ ::FindWindowA("Notepad",0) };
	//DWORD pid{};
	//DWORD tid{ ::GetWindowThreadProcessId(hWnd, &pid) };
	//HANDLE hProcess{ ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid) };
	//::_tprintf(_T("Window Handle : %p, Process ID : %x, Process Addr : %p\n"), hWnd, pid, hProcess);

	std::array<DWORD, PROCESS_CNT> processIDs{};
	DWORD cbNeeded{};
	HANDLE hProcess{};
	
	if (::EnumProcesses(processIDs.data(), static_cast<DWORD>(PROCESS_CNT), &cbNeeded))
	{
		DWORD nProcesses{ cbNeeded / sizeof(DWORD) };
		for (DWORD i = 0; i < nProcesses; ++i)
		{
			hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIDs[i]);
			if (hProcess != NULL)
			{
				TCHAR szBuffer[MAX_PATH + _MAX_FNAME]{};
				::ZeroMemory(szBuffer, MAX_PATH + _MAX_FNAME);
				if (::GetModuleFileNameEx(hProcess, NULL, szBuffer, MAX_PATH + _MAX_FNAME) != NULL)
				{
					::_tprintf(_T("%d. [PID : %d] %s\n"), i, processIDs[i], szBuffer);
				}
				CloseHandle(hProcess);
			}
		}

		DWORD idx{};
		do
		{
			::_tprintf(_T("Select Injection Target Process Index\n"));
			std::cin >> idx;
		} while (idx < 0 || nProcesses <= idx);

		hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processIDs[idx]);
	}

	// 2. 인젝션 할 dll 선택
	std::string targetFile{};
	ComInit com{};
	CComPtr<IFileOpenDialog> dlg{};

	SUCCEEDED(dlg.CoCreateInstance(CLSID_FileOpenDialog));

	// 파일 선택, 다중선택 허용
	DWORD opts{};
	dlg->GetOptions(&opts);
	dlg->SetOptions(opts | FOS_FILEMUSTEXIST | FOS_FORCEFILESYSTEM);

	dlg->SetTitle(_T("Inject할 dll 파일을 선택해 주세요."));

	// 파일 타입 필터: .dll 전용
	const COMDLG_FILTERSPEC filter{ L"Dll Files (*.dll)", L"*.dll" };
	dlg->SetFileTypes(1, &filter);
	dlg->SetFileTypeIndex(1);

	SUCCEEDED(dlg->Show(nullptr));

	CComPtr<IShellItemArray> itemArray{};
	dlg->GetResults(&itemArray);

	if (!itemArray) return 0;

	DWORD count{};
	itemArray->GetCount(&count);

	for (DWORD i = 0; i < count; ++i)
	{
		CComPtr<IShellItem> item{};
		itemArray->GetItemAt(i, &item);
		if (!item) continue;

		LPWSTR path{};
		if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, &path)))
		{
			int len{ lstrlenW(path) };
			targetFile.resize(len, '\0');
			WideCharToMultiByte(
				CP_UTF8, 0,
				path, len,
				&targetFile[0], len,
				nullptr, nullptr
			);
		}
		CoTaskMemFree(path);
	}

	std::cout << targetFile << std::endl;

	// 3. LoadLibrary 주소 구하기
	HMODULE hDll{ ::GetModuleHandleA("Kernel32.dll") };
	PTHREAD_START_ROUTINE f{ reinterpret_cast<PTHREAD_START_ROUTINE>(::GetProcAddress(hDll, "LoadLibraryA")) };
	::_tprintf(_T("Dll Addr : %p, Func Addr : %p\n"), hDll, f);

	// 4. 문자열 복사
	char* p{ reinterpret_cast<char*>(::VirtualAllocEx(hProcess, 0, targetFile.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE))};
	SIZE_T sz{};
	WriteProcessMemory(hProcess, p, targetFile.data(), targetFile.size(), &sz);

	// 5. 프로세스에 스레드 생성해서 LoadLibraryA 실행
	HANDLE hThread{ ::CreateRemoteThread(hProcess, 0, 0, f, p, 0, 0) };

	CloseHandle(hThread);
	CloseHandle(hProcess);
}