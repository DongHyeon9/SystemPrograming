#include "Client_UDP.h"

bool Client_UDP::Initialize()
{
    std::cout << "Initialize Client UDP" << std::endl;

    if (!__super::Initialize()) return false;

    // 1. 소켓 생성
    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket == INVALID_SOCKET)
    {
        std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    // 서버정보 입력
    SOCKADDR_IN serverAddr{};
    serverAddr.sin_family = AF_INET;
    ::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = ::htons(7777);

    // 2. UDP 연결
    if (::connect(socket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cout << "Connect Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

void Client_UDP::Deinitialize()
{
    ::closesocket(socket);
    __super::Deinitialize();
}

void Client_UDP::Run()
{
    std::string sendBuffer{ "Hello Echo" };

    while (true)
    {
        if (::send(socket, sendBuffer.c_str(), static_cast<int32>(sendBuffer.size()), 0) == SOCKET_ERROR)
        {
            std::cout << "Send Error Code : " << ::WSAGetLastError() << std::endl;
            return;
        }

        std::cout << "Send Data! Data = " << sendBuffer << " / " << sendBuffer.size() << std::endl;

        std::string recvBuffer(RECV_SIZE, 0);

        if (::recv(socket, recvBuffer.data(), static_cast<int32>(recvBuffer.size()), 0) <= 0)
        {
            std::cout << "Recv Error Code : " << ::WSAGetLastError() << std::endl;
            return;
        }

        std::cout << "Recv Data! Data = " << recvBuffer << " / " << recvBuffer.size() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
