#pragma once
#include "AppBase.h"

// Overlapped 모델 (Completion Routine 콜백 기반)
// - 비동기 입출력 지원하는 소켓 생성
// - 비동기 입출력 함수 호출 (완료 루틴의 시작 주소를 넘겨준다)
// - 비동기 작업이 바로 완료되지 않으면, WSA_IO_PENDING 오류 코드
// - 비동기 입출력 함수 호출한 쓰레드를 -> Alertable Wait 상태로 만든다
// ex) WaitForSingleObjectEx, WaitForMultipleObjectsEx, SleepEx, WSAWAitForMultipleEvents
// - 비동기 IO 완료되면, 운영체제는 완료 루틴 호출
// - 완료 루틴 호출이 모두 끝나면, 쓰레드는 Alertable Wait 상태에서 빠져나온다

//void CompletionRoutine()
// 1) 오류 발생시 0 아닌 값
// 2) 전송 바이트 수
// 3) 비동기 입출력 함수 호출 시 넘겨준 WSAOVERLAPPED 구조체의 주소값
// 4) 0

// Overlapped (콜백 기반)
// - 장점) 성능
// - 단점) 모든 비동기 소켓 함수에서 사용 가능하진 않음 (accept). 빈번한 Alertable Wait으로 인한 성능 저하
class NETWORK_API Server_Overlapped_Callback : public AppBase<Server_Overlapped_Callback>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};
