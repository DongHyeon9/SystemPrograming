#include <iostream>
#include <Windows.h>
#include <DbgHelp.h>

bool BeginDump();
bool EndDump();
LONG WINAPI UnHandledExceptionFilter(_EXCEPTION_POINTERS* pExceptionInfo); // 콜백 함수

// 최상위 예외 처리기 백업할 전역 변수
LPTOP_LEVEL_EXCEPTION_FILTER PREV_EXCEPTION_FILTER{ NULL };

bool BeginDump()
{
	// 1번 단계
	// SetErrorMode: 프로그램 에러 발생 시 시스템에서 발생하는 메시지/대화상자 발생 여부를 제어
	// SEM_FAILCRITICALERRORS: 에러 발생 시 시스템에서 처리하지 않고 프로세스에게 오류를 전달 (대신 처리하도록)
	::SetErrorMode(SEM_FAILCRITICALERRORS);

	// 2번 단계
	// SetUnhandledExceptionFilter: 크래쉬 발생 시 인자로 전달한 함수가 콜백된다. 인자로 전달한 함수가 최상위 예외 처리기가 되는 듯 하다.
	// 크래쉬 발생 시 크래쉬 위치 정보를 얻기 위함이라 한다.
	// 반환 값은 이전 최상위 예외 처리기이다.
	PREV_EXCEPTION_FILTER = ::SetUnhandledExceptionFilter(UnHandledExceptionFilter);

	return true;
}

bool EndDump()
{
	// 설정 기존으로 되돌리기
	::SetUnhandledExceptionFilter(PREV_EXCEPTION_FILTER);
	return true;
}

///////////////
// 3번 단계
///////////////

// Unhandled exception 발생 시 호출될 콜백 함수 타입
using MINIDUMPWRITEDUMP = BOOL(WINAPI*)(
	HANDLE,
	DWORD,
	HANDLE,
	MINIDUMP_TYPE,
	CONST PMINIDUMP_EXCEPTION_INFORMATION,
	CONST PMINIDUMP_USER_STREAM_INFORMATION,
	CONST PMINIDUMP_CALLBACK_INFORMATION
	);

LONG WINAPI UnHandledExceptionFilter(_EXCEPTION_POINTERS* pExceptionInfo)
{
	// 덤프 데이터 write에 필요한 함수를 담고 있는 dll 로드
	HMODULE hDll{ ::LoadLibrary(TEXT("DBGHELP.DLL")) };
	if (hDll != NULL)
	{
		// dll에서 MiniDumpWriteDump 함수 가져온다.
		MINIDUMPWRITEDUMP WriteDumpFunc{ reinterpret_cast<MINIDUMPWRITEDUMP>(::GetProcAddress(hDll, "MiniDumpWriteDump")) };
		if (WriteDumpFunc != NULL)
		{
			// 덤프 파일 이름 설정 ( MyWinFramework_YYMMDD_HHMMSS.dmp )
			// 덤프 파일은 exe 파일의 버전 및 빌드와 일치해야 하기 때문에 exe 파일의 버전을 얻을 수 있다면 그 정보를 활용할 수 있으면 하면 좋다.
			// 근데 우선은 날짜 시간만
			TCHAR DumpName[MAX_PATH]{ NULL, };

			SYSTEMTIME SystemTime{};
			::ZeroMemory(&SystemTime, sizeof(SystemTime));
			::GetLocalTime(&SystemTime);

			::_snwprintf_s(DumpName, MAX_PATH, TEXT("MyWinFramework_%02d%02d%02d_%02d%02d%02d.dmp"),
				SystemTime.wYear % 1000, SystemTime.wMonth, SystemTime.wDay,
				SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond
			);


			// 파일 생성
			HANDLE hDump{ ::CreateFile(DumpName,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL) };

			// 덤프에 exception 정보 write
			if (hDump != NULL)
			{
				MINIDUMP_EXCEPTION_INFORMATION DumpExceptionInfo{};
				::ZeroMemory(&DumpExceptionInfo, sizeof(DumpExceptionInfo));

				DumpExceptionInfo.ThreadId = ::GetCurrentThreadId();
				DumpExceptionInfo.ExceptionPointers = pExceptionInfo;
				DumpExceptionInfo.ClientPointers = FALSE;

				const DWORD fullDumpFlags
				{
					MINIDUMP_TYPE::MiniDumpWithFullMemory |
					MINIDUMP_TYPE::MiniDumpWithFullMemoryInfo |
					MINIDUMP_TYPE::MiniDumpWithHandleData |
					MINIDUMP_TYPE::MiniDumpWithUnloadedModules |
					MINIDUMP_TYPE::MiniDumpWithThreadInfo
				};

				MINIDUMP_TYPE miniDumpFlag{ MINIDUMP_TYPE::MiniDumpNormal };

				BOOL bSuccess{ WriteDumpFunc(
					::GetCurrentProcess(), 
					::GetCurrentProcessId(),
					hDump, 
					static_cast<MINIDUMP_TYPE>(fullDumpFlags),
					&DumpExceptionInfo,
					NULL, 
					NULL) };
				::CloseHandle(hDump);

				if (bSuccess == TRUE)
					return EXCEPTION_EXECUTE_HANDLER;
				else
					return EXCEPTION_CONTINUE_SEARCH;
			}
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

int main()
{
	BeginDump();

	// 실행코드

	EndDump();
}