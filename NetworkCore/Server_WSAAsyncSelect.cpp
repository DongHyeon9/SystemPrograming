#include "Server_WSAAsyncSelect.h"

#define WM_SOCKET (WM_USER+1)

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_SOCKET)
	{
		SOCKET clinetSocket{};
		const int32 event{ WSAGETSELECTEVENT(lParam) };
		const int32 error{ WSAGETSELECTERROR(lParam) };

		if (error != 0)
		{
			clinetSocket = static_cast<SOCKET>(wParam);
			std::cout << "Client Socket Error : " << error << std::endl;
			::closesocket(clinetSocket);
			return 0;
		}

		switch (event)
		{
		case FD_ACCEPT:
		{
			SOCKADDR_IN clientAddr{};
			int32 addrLen{ sizeof(SOCKADDR_IN) };
			clinetSocket = ::accept(Server_WSAAsyncSelect::GetInstance()->socket, reinterpret_cast<SOCKADDR*>(&clientAddr), &addrLen);
			if (clinetSocket == INVALID_SOCKET)
			{
				std::cout << "Accept Error Code : " << ::WSAGetLastError() << std::endl;
				break;
			}

			std::cout << "Client Connected! IP = " << GetIP(clientAddr) << std::endl;

			if (::WSAAsyncSelect(clinetSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE) == SOCKET_ERROR)
			{
				std::cout << "Client WSAAsyncSelect Error Code : " << ::WSAGetLastError() << std::endl;
			}
			break;
		}
		case FD_READ:
		{
			std::string buffer(RECV_SIZE, 0);
			clinetSocket = static_cast<SOCKET>(wParam);
			const int32 bytes{ ::recv(clinetSocket, buffer.data(), RECV_SIZE, 0) };
			if (bytes <= 0)
			{
				std::cout << "Recv Error Code : " << ::WSAGetLastError() << std::endl;
				break;
			}

			std::cout << "Recv Data! Data = " << buffer << " / " << buffer.size() << std::endl;

			break;
		}
		case FD_CLOSE:
		{
			clinetSocket = static_cast<SOCKET>(wParam);
			::closesocket(clinetSocket);
			break;
		}
		}
	}

	return ::DefWindowProc(hWnd, Message, wParam, lParam);
}

bool Server_WSAAsyncSelect::Initialize()
{
	std::cout << "Initialize Server WSAAsyncSelect" << std::endl;

	if (!__super::Initialize()) return false;

	// 1. 윈도우 클래스 등록
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"AsyncSelectServer";
	::RegisterClassEx(&wc);

	// 2. 메시지 윈도우 생성
	hWnd = CreateWindowEx(0, wc.lpszClassName, L"Server", 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);

	// 3. 소켓 생성
	socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (socket == INVALID_SOCKET)
	{
		std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	// 4. 소켓 모델 변경
	if (::WSAAsyncSelect(socket, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
	{
		std::cout << "Server WSAAsyncSelect Error Code : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	// 서버정보 입력
	SOCKADDR_IN serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	serverAddr.sin_port = ::htons(7777);

	// 5. 서버 소켓 정보 입력
	if (::bind(socket, reinterpret_cast<const SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cout << "Bind Error Code : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	// 6. 서버 소켓 가동
	if (::listen(socket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Listen Error Code : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	return true;
}

void Server_WSAAsyncSelect::Deinitialize()
{
	::closesocket(socket);
	__super::Deinitialize();
}

void Server_WSAAsyncSelect::Run()
{
	// 간단한 구현을 위해 GetMessage 사용
	MSG msg{};
	while (::GetMessage(&msg, NULL, 0, 0) > 0)
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}
