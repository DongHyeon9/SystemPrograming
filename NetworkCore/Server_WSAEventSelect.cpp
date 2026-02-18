#include "Server_WSAEventSelect.h"

bool Server_WSAEventSelect::Initialize()
{
	std::cout << "Initialize Server WSAEventSelect" << std::endl;

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

void Server_WSAEventSelect::Deinitialize()
{
	__super::Deinitialize();
}

void Server_WSAEventSelect::Run()
{
    std::vector<WSAEVENT> wsaEvents{};
    std::vector<Session> sessions{};

    sessions.reserve(100);
    WSAEVENT serverEvent{ ::WSACreateEvent() };
    wsaEvents.emplace_back(serverEvent);
    sessions.emplace_back(Session{ socket });

    if (::WSAEventSelect(socket, serverEvent, FD_ACCEPT | FD_CLOSE) == SOCKET_ERROR)
    {
        std::cout << "Server WSAEventSelect Error Code : " << ::WSAGetLastError() << std::endl;
        return;
    }

    while (true)
    {
		uint32 idx{ ::WSAWaitForMultipleEvents(static_cast<uint32>(wsaEvents.size()),wsaEvents.data(),FALSE,WSA_INFINITE,FALSE) };
        if (idx == WSA_WAIT_FAILED)
            continue;

        idx -= WSA_WAIT_EVENT_0;

        //::WSAResetEvent(wsaEvents[idx]);

        WSANETWORKEVENTS networkEvents{};
        if (::WSAEnumNetworkEvents(sessions[idx].socket, wsaEvents[idx], &networkEvents) == SOCKET_ERROR)
            continue;

        // 서버 소켓 체크
        if (networkEvents.lNetworkEvents & FD_ACCEPT)
        {
            // 에러 체크
            if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
                continue;

            Session clientSession{};
            int32 addrLen{ sizeof(SOCKADDR_IN) };
            clientSession.socket = ::accept(socket, reinterpret_cast<SOCKADDR*>(&clientSession.addr), &addrLen);
            if (clientSession.socket != INVALID_SOCKET)
            {
                std::cout << "Client Connected! IP = " << GetIP(clientSession.addr) << std::endl;
                WSAEVENT clientEvent{ ::WSACreateEvent() };
                wsaEvents.emplace_back(clientEvent);
                sessions.emplace_back(clientSession);
                if (::WSAEventSelect(clientSession.socket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
                {
                    std::cout << "Client WSAEventSelect Error Code : " << ::WSAGetLastError() << std::endl;
                    return;
                }
            }
        }

        // 클라 소켓 체크
        if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
        {
            // 에러 체크
            if ((networkEvents.lNetworkEvents & FD_READ) && (networkEvents.iErrorCode[FD_READ_BIT] != 0))
                continue;
            if ((networkEvents.lNetworkEvents & FD_WRITE) && (networkEvents.iErrorCode[FD_WRITE_BIT] != 0))
                continue;

            Session& s{ sessions[idx] };

            // 읽기
            if (s.recvBytes == 0)
            {
                const int32 recvLen{ ::recv(s.socket,s.buffer.data(),RECV_SIZE,0) };
                if (recvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
                {
                    // 연결 종료
                    sessions.erase(sessions.begin() + idx);
                    auto eventIter{ wsaEvents.begin() + idx };
                    ::WSACloseEvent(*eventIter);
                    wsaEvents.erase(eventIter);
                    continue;
                }

                s.recvBytes = recvLen;
                std::cout << "Recv Data! Data = " << s.buffer << " / " << s.buffer.size() << std::endl;
            }

            if (s.recvBytes > s.sendBytes)
            {
                const int32 sendLen{ ::send(s.socket,&s.buffer[s.sendBytes],s.recvBytes - s.sendBytes,0) };
                if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK)
                {
                    // 연결 종료
                    sessions.erase(sessions.begin() + idx);
                    auto eventIter{ wsaEvents.begin() + idx };
                    ::WSACloseEvent(*eventIter);
                    wsaEvents.erase(eventIter);
                    continue;
                }

                s.sendBytes += sendLen;
                if (s.recvBytes == s.sendBytes)
                {
                    s.recvBytes = 0;
                    s.sendBytes = 0;
                }

                std::cout << "Send Data! Data = " << s.buffer << " / " << s.buffer.size() << std::endl;
            }
        }

        if (networkEvents.lNetworkEvents & FD_CLOSE)
        {
            // 연결 종료
            sessions.erase(sessions.begin() + idx);
            auto eventIter{ wsaEvents.begin() + idx };
            ::WSACloseEvent(*eventIter);
            wsaEvents.erase(eventIter);
        }
    }

    for (auto& event : wsaEvents)
    {
        ::WSACloseEvent(event);
    }

    for (auto& s : sessions)
    {
        ::closesocket(s.socket);
    }
}
