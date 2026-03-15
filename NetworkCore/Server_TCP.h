#pragma once
#include "AppBase.h"

// 대표적인 TCP 장애 유형
// 인터넷 환경은 안정적이지 않기 때문에 네트워크 통신을 할 때 다양한 장애에 대해 대비해야 된다
// 
// =====어플리케이션 환경=====
// 1. Zero Window: 수신 측이 버퍼가 가득 차서 더 이상 데이터를 받을 수 없는 상태, 송신 측이 데이터를 전송할 수 없게 된다
// 
// =====네트워크 환경=====
// 2. 패킷 손실 (Packet Loss): 네트워크에서 패킷이 손실되는 현상
// 3. TCP Out-of-Order: 네트워크에서 순서대로 도착하지 않는 현상
// 4. Retransmission Timeout: 일정 시간 내에 ACK를 받지 못할 때, 송신 측이 패킷을 재전송하는 현상
// 5. Duplicate ACKs: 수신 측이 동일한 ACK를 여러 번 보내는 현상

// Protocol 설계
// TCP는 Byte Stream으로 데이터가 전송되기 때문에
// send와 recv는 1:1로 대응되지 않는다
// 그래서 protocol 설계 시, 데이터의 시작과 끝을 구분할 수 있는 방법이 필요하다
// 1. Delimiter-based Protocol: 데이터의 시작과 끝을 구분하는 구분자(Delimiter)를 사용하는 방법
// 2. Length-based Protocol: 데이터의 길이를 명시하는 방법

// 블로킹 방식의 TCP 소켓모델
class NETWORK_API Server_TCP : public AppBase<Server_TCP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	SOCKET socket{};

	SOCKET clientSocket{};
};

