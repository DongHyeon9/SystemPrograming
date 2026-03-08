#pragma once
#include "AppBase.h"

// RIO (Registered I/O) 모델
// Windows 8에서 도입된 고성능 네트워킹 API
// IOCP보다 더 낮은 레이턴시와 높은 처리량을 제공하도록 설계됨
// RIO는 커널에서 사용자 공간으로 직접 I/O 작업을 수행하는 방식으로, Page Lock 문제를 완전히 해결
// Request Queue와 Completion Queue를 사용하여 I/O 작업을 관리
//  -> User-mode polling을 지원하여, 시스템 콜과 컨텍스트 스위칭을 줄여 성능 향상

class NETWORK_API Server_RIO : public AppBase<Server_RIO>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

};

