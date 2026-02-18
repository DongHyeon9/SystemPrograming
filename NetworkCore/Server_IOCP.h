#pragma once
#include "AppBase.h"

// Reactor Pattern (준비되면 알려줘 -> 내가 직접 읽고 처리)
//  - 동기적 이벤트 처리 방식
//  - 루프를 돌면서 여러 I/O를 감시
//  - 프로그램에서 직접 I/O작업을 수행 -> 핸들러가 실제 작업을 실행
// 
// Proactor Pattern (읽어와서 알려줘 -> OS가 읽어오면 처리)
// - Completion 기반 비동기 처리 방식
// - 프로그램 I/O요청 -> OS 완료 및 통지

// Overlapped 모델 (Completion Routine 콜백 기반)
// - 비동기 입출력 함수 완료되면, 쓰레드마다 있는 APC(Asynchronous procedure call) 큐에 일감이 쌓임 -> https://learn.microsoft.com/ko-kr/windows/win32/sync/asynchronous-procedure-calls
// - Alertable Wait 상태로 들어가서 APC 큐 비우기 (콜백 함수)
// 단점) APC큐 쓰레드마다 있다! Alertable Wait 자체도 조금 부담!
// 단점) 이벤트 방식 소켓:이벤트 1:1 대응

// IOCP (Completion Port) 모델
// - APC -> Completion Port (쓰레드마다 있는건 아니고 1개. 중앙에서 관리하는 APC 큐?)
// - Alertable Wait -> CP 결과 처리를 GetQueuedCompletionStatus
// 쓰레드랑 궁합이 굉장히 좋다!

class NETWORK_API Server_IOCP : public AppBase<Server_IOCP>
{
public:
	// std::vector<std::thread> workerThread의 복사 생성자 때문에 추가
	Server_IOCP() = default;
	Server_IOCP(const Server_IOCP&) = delete;
	Server_IOCP& operator=(const Server_IOCP&) = delete;

	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	void WorkerRun();

private:
	SOCKET socket{};
	HANDLE hIOCP{};
	std::vector<std::thread> workerThread{};
	bool bTerminate{};
};

