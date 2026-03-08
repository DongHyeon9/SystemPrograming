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

// Page Locking
// I/O 작업이 완료될 때까지 해당 버퍼가 Page Out되지 않도록 고정하는 기술
// WSARecv, WSASend는 호출 시 사용자가 제공한 버퍼에 데이터를 직접 전달
// -> 실제 I/O는 커널에서 수행(커널이 유저 메모리 버퍼에 직접 접근)
// 가상 메모리 시스템 때문에 사용자가 제공한 버퍼가 Page Out될 수 있다(커널이 접근하려는 메모리가 사라질 수 있다)
// Page Lock은 버퍼크기 단위가 아니라 Page단위로 Lock을 한다(Windows의 Page 크기는 일반적으로 4KB)
// 
// 문제점
// IOCP는 Recv를 미리 걸어두는 구조라서 실제로 데이터가 안 와도 Page Lock이 발생한다
// 클라이언트가 10,000명 접속 했다고 가정하면 10,000 * 4KB = 40MB의 메모리가 Page Lock된다
// -> 메모리 낭비 심각
// 
// 완화 방법(Memory Pool + Object Pool)
// 이렇게 하면 Page Lock이 발생하는 버퍼의 개수를 줄일 수 있다
// 
// 해결법(Zero Byte Recv)
// 데이터가 실제로 올 때만 버퍼를 준비
// 
// ZeroByteRecv는 대부분 쓰이지 않음
// 1. Page Lock으로 인한 메모리 부족 문제가 거의 안터짐 (RAM이 충분히 크고, 메모리 풀로 관리하면 충분히 완화 가능)
// 2. ZeroByteRecv는 구현이 까다롭고, 성능이 떨어질 수 있다 (추가적인 시스템 콜과 복잡한 로직 필요)
// 3. 더 좋은 기술 등장(예: RIO)

class NETWORK_API Server_IOCP : public AppBase<Server_IOCP>
{
public:
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

