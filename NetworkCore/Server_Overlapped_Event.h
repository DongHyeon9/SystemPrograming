#pragma once
#include "AppBase.h"

// Overlapped IO (비동기 + 논블로킹)
// - Overlapped 함수를 건다 (WSARecv, WSASend)
// - Overlapped 함수가 성공했는지 확인 후
// -> 성공했으면 결과 얻어서 처리
// -> 실패했으면 사유를 확인

// WSASend ,WSARecv
// 1) 비동기 입출력 소켓
// 2) WSABUF 배열의 시작 주소 + 개수 // Scatter-Gather
// 3) 보내고/받은 바이트 수
// 4) 상세 옵션인데 0
// 5) WSAOVERLAPPED 구조체 주소값
// 6) 입출력이 완료되면 OS가 호출할 콜백 함수

// Overlapped 모델 (이벤트 기반)
// - 비동기 입출력 지원하는 소켓 생성 + 통지 받기 위한 이벤트 객체 생성
// - 비동기 입출력 함수 호출 (1에서 만든 이벤트 객체를 같이 넘겨줌)
// - 비동기 작업이 바로 완료되지 않으면, WSA_IO_PENDING 오류 코드
// 운영체제는 이벤트 객체를 signaled 상태로 만들어서 완료 상태 알려줌
// - WSAWaitForMultipleEvents 함수 호출해서 이벤트 객체의 signal 판별
// - WSAGetOverlappedResult 호출해서 비동기 입출력 결과 확인 및 데이터 처리

// WSAGetOverlappedResult
// 1) 비동기 소켓
// 2) 넘겨준 overlapped 구조체
// 3) 전송된 바이트 수
// 4) 비동기 입출력 작업이 끝날때까지 대기할지? -> false
// 5) 비동기 입출력 작업 관련 부가 정보. 거의 사용 안 함.

// Overlapped (이벤트 기반)
// - 장점) 성능
// - 단점) 64개 제한
class NETWORK_API Server_Overlapped_Event : public AppBase<Server_Overlapped_Event>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

