#include "Server_TCP.h"

bool Server_TCP::Initialize()
{
    std::cout << "Initialize Server TCP" << std::endl;

    if (!__super::Initialize()) return false;

    // 1. 소켓 생성
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket == INVALID_SOCKET)
    {
        std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    // 서버정보 입력
    SOCKADDR_IN serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    serverAddr.sin_port = ::htons(7777);

    // 2. 서버 소켓 정보 입력
    if (::bind(socket, reinterpret_cast<const SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Bind Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    // 3. 서버 소켓 가동
    if (::listen(socket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Listen Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

void Server_TCP::Deinitialize()
{
    ::closesocket(clientSocket);
    ::closesocket(socket);
    __super::Deinitialize();
}

void Server_TCP::Run()
{
    while (true)
    {
        // 4. 클라이언트 소켓 연결
        SOCKADDR_IN clientAddr{};
        int32 addrLen{ sizeof(clientAddr) };
        clientSocket = ::accept(socket, reinterpret_cast<SOCKADDR*>(&clientAddr), &addrLen);
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
            if (::recv(clientSocket, buffer.data(), static_cast<int32>(RECV_SIZE), 0) <= 0)
            {
                std::cout << "Recv Error Code : " << ::WSAGetLastError() << std::endl;
                return;
            }

            std::cout << "Recv Data! Data = " << buffer << " / " << buffer.size() << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));


            if (::send(clientSocket, buffer.c_str(), static_cast<int32>(RECV_SIZE), 0) == SOCKET_ERROR)
            {
                std::cout << "Send Error Code : " << ::WSAGetLastError() << std::endl;
                return;
            }

            std::cout << "Send Data! Data = " << buffer << " / " << buffer.size() << std::endl;
        }
    }
}
