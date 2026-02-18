#include "Server_NBS.h"

bool Server_NBS::Initialize()
{
    std::cout << "Initialize Server Non-Blocking TCP" << std::endl;

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

void Server_NBS::Deinitialize()
{
    ::closesocket(clientSocket);
    ::closesocket(socket);
    __super::Deinitialize();
}

void Server_NBS::Run()
{
    while (true)
    {
        // 5. 클라이언트 소켓 연결
        SOCKADDR_IN clientAddr{};
        int32 addrLen{ sizeof(clientAddr) };
        clientSocket = ::accept(socket, reinterpret_cast<SOCKADDR*>(&clientAddr), &addrLen);
        if (clientSocket == INVALID_SOCKET)
        {
            if (::WSAGetLastError() == WSAEWOULDBLOCK)
                continue;

            std::cout << "Accept Error Code : " << ::WSAGetLastError() << std::endl;
            break;
        }

        const bool bAccpeted{ clientSocket != INVALID_SOCKET };
        if (bAccpeted)
        {
            std::string ipAddress(IP_ADDR_SIZE, 0);
            ::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress.data(), IP_ADDR_SIZE);
            std::cout << "Client Connected! IP = " << ipAddress << std::endl;
        }
        else
        {
            std::cout << "Accept Error Code : " << ::WSAGetLastError() << std::endl;
        }

        while (bAccpeted)
        {
            std::string buffer(RECV_SIZE, 0);
            const int32 recvLen{ ::recv(clientSocket, buffer.data(), static_cast<int32>(RECV_SIZE), 0) };
			if (recvLen == SOCKET_ERROR)
			{
                if (::WSAGetLastError() == WSAEWOULDBLOCK)
                    continue;

				std::cout << "Recv Error Code : " << ::WSAGetLastError() << std::endl;
                return;
			}
            else if (recvLen == 0)
            {
                std::cout << "Disconnected" << std::endl;
                return;
            }

			std::cout << "Recv Data! Data = " << buffer << " / " << buffer.size() << std::endl;
            
            std::this_thread::sleep_for(std::chrono::seconds(1));

            while (true)
            {
                if (::send(clientSocket, buffer.c_str(), static_cast<int32>(RECV_SIZE), 0) == SOCKET_ERROR)
                {
                    if (::WSAGetLastError() == WSAEWOULDBLOCK)
                        continue;

                    std::cout << "Send Error Code : " << ::WSAGetLastError() << std::endl;
                    return;
                }

                std::cout << "Send Data! Data = " << buffer << " / " << buffer.size() << std::endl;
                break;
            }
        }
    }
}
