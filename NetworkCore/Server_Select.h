#pragma once
#include "AppBase.h"

// Select 모델 = (select 함수가 핵심이 되는)
// 소켓 함수 호출이 성공할 시점을 미리 알 수 있다!
// 문제 상황)
// 수신버퍼에 데이터가 없는데, read 한다거나!
// 송신버퍼가 꽉 찼는데, write 한다거나!
// - 블로킹 소켓 : 조건이 만족되지 않아서 블로킹되는 상황 예방
// - 논블로킹 소켓 : 조건이 만족되지 않아서 불필요하게 반복 체크하는 상황을 예방

// socket set
// 1) 읽기[ 2 ] 쓰기[ ] 예외(OOB)[ ] 관찰 대상 등록
// OutOfBand는 send() 마지막 인자 MSG_OOB로 보내는 특별한 데이터
// 받는 쪽에서도 recv OOB 세팅을 해야 읽을 수 있음
// 2) select(readSet, writeSet, exceptSet); -> 관찰 시작
// 3) 적어도 하나의 소켓이 준비되면 리턴 -> 낙오자는 알아서 제거됨
// 4) 남은 소켓 체크해서 진행

// fd_set set;
// FD_ZERO : 비운다
// ex) FD_ZERO(set);
// FD_SET : 소켓 s를 넣는다
// ex) FD_SET(s, &set);
// FD_CLR : 소켓 s를 제거
// ex) FD_CLR(s, &set);
// FD_ISSET : 소켓 s가 set에 들어있으면 0이 아닌 값을 리턴한다

// Select 방식의 소켓
// - 장점) 윈도우/리눅스 공통. 
// - 단점) 성능 최하 (매번 등록 비용), 64개 제한
class NETWORK_API Server_Select : public AppBase<Server_Select>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};
};

