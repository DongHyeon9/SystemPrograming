#include "ClientApp.h"

bool ClientApp::Initialize()
{
    // 1. 모듈 초기화
    InitializeModule();
    // 2. 소켓 생성
    CreateSocket();
    // 3. 서버 연결
    ConnectToServer();

    return true;
}

void ClientApp::Deinitialize()
{
    ::closesocket(clientSocket);
    ::WSACleanup();
    ::FreeLibrary(module);
}

bool ClientApp::InitializeModule()
{
    WSAData wsaData{};
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Fail WSAStartup : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    module = ::LoadLibrary(MODULE_NAME);
    if (!module)
    {
        std::cout << "Fail LoadLibrary : " << ::GetLastError() << std::endl;
        return false;
    }

    return true;
}

bool ClientApp::CreateSocket()
{
    clientSocket = NETWORK_INTERFACE::CREATE_SOCKET_FUNC();
    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

bool ClientApp::ConnectToServer()
{
    // 서버정보 기입
    serverAddr.sin_family = AF_INET;
    ::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = ::htons(7777);

    if (NETWORK_INTERFACE::CONNECT_FUNC(clientSocket, serverAddr) == SOCKET_ERROR)
    {
        std::cout << "Connect Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Connected To Server!" << std::endl;
    return true;
}

bool ClientApp::Send(const std::string& SendData)
{
    if (NETWORK_INTERFACE::SEND_FUNC(clientSocket, SendData) == SOCKET_ERROR)
    {
        std::cout << "Send Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Send Data! Data = " << SendData << " / " << SendData.size() << std::endl;

    return true;
}

bool ClientApp::Recv(std::string& Recv, size_t BufferSize)
{
    int recvLen{ NETWORK_INTERFACE::RECV_FUNC(clientSocket, Recv, BufferSize) };
    if (recvLen <= 0)
    {
        std::cout << "Recv Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Recv Data! Data = " << Recv << " / " << Recv.size() << std::endl;
    return true;
}
