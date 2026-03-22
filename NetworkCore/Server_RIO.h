#pragma once
#include "AppBase.h"

// RIO (Registered I/O) 모델
// Windows 8에서 도입된 고성능 네트워킹 API
// IOCP보다 더 낮은 레이턴시와 높은 처리량을 제공하도록 설계됨
// RIO는 커널에서 사용자 공간으로 직접 I/O 작업을 수행하는 방식으로, Page Lock 문제를 해결
// Request Queue와 Completion Queue를 사용하여 I/O 작업을 관리
//  -> User-mode polling을 지원하여, 시스템 콜과 컨텍스트 스위칭을 줄여 성능 향상

// DMA(Direct Memory Access)
// DAM란 Hardware가 CPU의 개입 없이 사용자의 메모리에 직접 데이터를 읽고 쓸 수 있도록 하는 기술
// Hardware가 해당 기술을 지원하는지가 중요
// 운영체제는 해당 기술이 적용가능한 hardware를 감지해서 DMA가 되도록 지원

class NETWORK_API Server_RIO : public AppBase<Server_RIO>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

};

