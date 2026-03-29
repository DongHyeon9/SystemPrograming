#pragma once
#include "AppBase.h"

class NETWORK_API Client_Asio_SyncTCP : public AppBase<Client_Asio_SyncTCP>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	// 생성자에서 변수 선언 순서에 의존하는 코드는 좋은 코드가 아니다...
	boost::asio::io_context ctx{};
	boost::asio::ip::tcp::socket socket{ ctx };
	
};