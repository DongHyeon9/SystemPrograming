#include "Server_Select.h"

bool Server_Select::Initialize()
{
	std::cout << "Initialize Server Select" << std::endl;

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

void Server_Select::Deinitialize()
{
    ::closesocket(socket);
	__super::Deinitialize();
}

void Server_Select::Run()
{
    std::vector<Session> sessions{};
    sessions.reserve(100);

    fd_set reads{};
    fd_set writes{};

    while (true)
    {
        // 소켓 셋 초기화
        FD_ZERO(&reads);
        FD_ZERO(&writes);

        // 서버 소켓 등록
        FD_SET(socket, &reads);

        // 클라이언트 소켓 등록
        for (auto& session : sessions)
        {
            if (session.recvBytes <= session.sendBytes)
                FD_SET(session.socket, &reads);
            else
                FD_SET(session.socket, &writes);
        }

        // 5. 관찰 시작
        if (::select(0, &reads, &writes, nullptr, nullptr) == SOCKET_ERROR)
        {
            std::cout << "Select Error Code : " << ::WSAGetLastError() << std::endl;
            return;
        }

        // 클라이언트 연결 확인
        if (FD_ISSET(socket, &reads))
        {
            SOCKADDR_IN clientAddr{};
            int32 addrLen{ sizeof(clientAddr) };
            SOCKET clientSocket{ ::accept(socket,reinterpret_cast<SOCKADDR*>(&clientAddr),&addrLen) };
            if (clientSocket != INVALID_SOCKET)
            {
                std::cout << "Client Connected! IP = " << GetIP(clientAddr) << std::endl;
                sessions.emplace_back(Session{ clientSocket, clientAddr });
            }
        }

        // 소켓 체크
        auto vIter = sessions.begin();
        while (vIter != sessions.end())
        {
            SOCKET& sSocket{ vIter->socket };
            std::string& sBuffer{ vIter->buffer };
            int32& sRecvBytes{ vIter->recvBytes };
            int32& sSendBytes{ vIter->sendBytes };

            // Send
            if (FD_ISSET(sSocket, &reads))
            {
                sRecvBytes = ::recv(sSocket, sBuffer.data(), static_cast<int32>(RECV_SIZE), 0);
                // 연결 종료
                if (sRecvBytes <= 0)
                {
                    vIter = sessions.erase(vIter);
                    continue;
                }

                std::cout << "Recv Data! Data = " << sBuffer << " / " << sBuffer.size() << std::endl;
            }

            // Write
            if (FD_ISSET(sSocket, &writes))
            {
                // 블로킹 모드 -> 모든 데이터 다 보냄
                // 논블로킹 모드 -> 일부만 보낼 수가 있음 (상대방 수신 버퍼 상황에 따라)
                const int32 sendLen{ ::send(sSocket, &sBuffer[sSendBytes], sRecvBytes - sSendBytes, 0) };
                // 연결 종료
                if (sendLen == SOCKET_ERROR)
                {
                    vIter = sessions.erase(vIter);
                    continue;
                }

                std::cout << "Send Data! Data = " << sBuffer << " / " << sBuffer.size() << std::endl;

                sSendBytes += sendLen;
                if (sSendBytes == sRecvBytes)
                {
                    sSendBytes = 0;
                    sRecvBytes = 0;
                }
            }
            ++vIter;
        }
    }
}
