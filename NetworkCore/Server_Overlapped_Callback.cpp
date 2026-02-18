#include "Server_Overlapped_Callback.h"

static constexpr DWORD BUFFER_COUNT{ 1 };

void CALLBACK RecvCallback(DWORD Error, DWORD RecvLen, LPWSAOVERLAPPED Overlapped, DWORD Flags)
{
    Session_Overlapped* session{ reinterpret_cast<Session_Overlapped*>(Overlapped) };
    std::cout << "Recv Data! Data = " << session->buffer << " / " << session->buffer.size() << std::endl;
}

bool Server_Overlapped_Callback::Initialize()
{
	std::cout << "Initialize Server Overlapped Callback" << std::endl;

	if (!__super::Initialize()) return false;

    // 1. 소켓 생성
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket == INVALID_SOCKET)
    {
        std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    // 2. 논블로킹 소켓 변환
    u_long mode{};
    if (::ioctlsocket(socket, FIONBIO, &mode) == INVALID_SOCKET)
    {
        std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    // 서버정보 입력
    SOCKADDR_IN serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    serverAddr.sin_port = ::htons(7777);

    // 3. 서버 소켓 정보 입력
    if (::bind(socket, reinterpret_cast<const SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Bind Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    // 4. 서버 소켓 가동
    if (::listen(socket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Listen Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

void Server_Overlapped_Callback::Deinitialize()
{
	__super::Deinitialize();
}

void Server_Overlapped_Callback::Run()
{
    Session_Overlapped session{};
    session.overlapped.hEvent = ::WSACreateEvent();
    while (true)
    {
        int32 addrLen{ sizeof(SOCKADDR_IN) };
        session.socket = ::accept(socket, reinterpret_cast<SOCKADDR*>(&session.addr), &addrLen);
        if (session.socket != INVALID_SOCKET)
            break;

        if (::WSAGetLastError() != WSAEWOULDBLOCK)
        {
            std::cout << "Accept Error Code : " << ::WSAGetLastError() << std::endl;
            return;
        }
    }

    std::cout << "Client Connected! IP = " << GetIP(session.addr) << std::endl;

    while (true)
    {
        WSABUF wsaBuf{};
        wsaBuf.buf = session.buffer.data();
        wsaBuf.len = RECV_SIZE;

        DWORD recvLen{};
        DWORD flags{};
        if (::WSARecv(session.socket, &wsaBuf, BUFFER_COUNT, &recvLen, &flags, &session.overlapped, RecvCallback) == SOCKET_ERROR)
        {
            if (::WSAGetLastError() == WSA_IO_PENDING)
            {
                // Pending
                // Alertable Wait
                ::SleepEx(INFINITE, TRUE);
            }
            else
            {
                std::cout << "WSARecv Error Code : " << ::WSAGetLastError() << std::endl;
                return;
            }
        }
    }

    ::closesocket(session.socket);
    ::WSACloseEvent(session.overlapped.hEvent);
}
