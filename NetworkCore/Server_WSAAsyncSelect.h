#pragma once
#include "AppBase.h"

// WSAAsyncSelect
// - 장점) 구현이 단순
// - 단점) 단인 윈도우 메시지 루프에 의존, 대량소켓 처리에 비적합
class NETWORK_API Server_WSAAsyncSelect : public AppBase<Server_WSAAsyncSelect>
{
public:
	bool Initialize()override;
	void Deinitialize()override;
	void Run()override;

private:
	HWND hWnd{};
	SOCKET socket{};

	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
};

