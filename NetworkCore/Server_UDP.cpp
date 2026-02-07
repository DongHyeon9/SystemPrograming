#include "Server_UDP.h"

bool Server_UDP::Initialize()
{
    std::cout << "Initialize Server UDP" << std::endl;

    if (!__super::Initialize()) return false;

    // 1. 家南 积己
    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket == INVALID_SOCKET)
    {
        std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    SOCKADDR_IN serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    serverAddr.sin_port = ::htons(7777);

    // 2. 家南 官牢靛
    if (::bind(socket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Bind Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

void Server_UDP::Deinitialize()
{
    ::closesocket(socket);
    __super::Deinitialize();
}

void Server_UDP::Run()
{
    while (true)
    {
        SOCKADDR_IN clientAddr{};
        int32 addrLen{ sizeof(clientAddr) };
        std::string buffer(RECV_SIZE, 0);

        const int32 recvLen{ ::recvfrom(socket, buffer.data(), static_cast<int32>(buffer.size()), 0, reinterpret_cast<SOCKADDR*>(&clientAddr), &addrLen) };
        if (recvLen <= 0)
        {
            std::cout << "Recv Error Code : " << ::WSAGetLastError() << std::endl;
            return;
        }

        std::cout << "Recv Data! Data = " << buffer << " / " << buffer.size() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (::sendto(socket, buffer.c_str(), static_cast<int32>(RECV_SIZE), 0, reinterpret_cast<SOCKADDR*>(&clientAddr), addrLen) == SOCKET_ERROR)
        {
            std::cout << "Send Error Code : " << ::WSAGetLastError() << std::endl;
            return;
        }

        std::cout << "Send Data! Data = " << buffer << " / " << buffer.size() << std::endl;
    }
}
