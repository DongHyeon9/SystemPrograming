#include "ServerApp.h"

bool ServerApp::Initialize()
{
    // 1. 모듈 초기화
    InitializeModule();
    // 2. 소켓 생성
    CreateSocket();
    // 3. 서버 소켓 정보 입력
    BindSocket();
    // 4. 서버 소켓 가동
    Listen();
    // 5. 클라이언트 소켓 연결
    AcceptClient();

    return true;
}

void ServerApp::Deinitialize()
{
    ::closesocket(clientSocket);
    ::closesocket(socket);
    ::WSACleanup();
    ::FreeLibrary(module);
}

bool ServerApp::InitializeModule()
{
    WSAData wsaData{};
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return false;

    module = ::LoadLibrary(MODULE_NAME);
    if (!module)
    {
        std::cout << "Fail LoadLibrary : " << ::GetLastError() << std::endl;
        return false;
    }

    return true;
}

bool ServerApp::CreateSocket()
{
    socket = NETWORK_INTERFACE::CREATE_SOCKET_FUNC();
    if (socket == INVALID_SOCKET)
    {
        std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool ServerApp::BindSocket()
{
    // 서버정보 기입
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    serverAddr.sin_port = ::htons(7777);

    if (NETWORK_INTERFACE::BIND_FUNC(socket, serverAddr) == SOCKET_ERROR)
    {
        std::cout << "Bind Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool ServerApp::Listen()
{
    if (NETWORK_INTERFACE::LISTEN_FUNC(socket) == SOCKET_ERROR)
    {
        std::cout << "Listen Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool ServerApp::AcceptClient()
{
    clientSocket = NETWORK_INTERFACE::ACCEPT_FUNC(socket, clientAddr, addrLen);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cout << "Accept Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    std::string ipAddress(IP_ADDR_SIZE, 0);
    ::inet_ntop(AF_INET, &clientAddr.sin_addr, ipAddress.data(), IP_ADDR_SIZE);
    std::cout << "Client Connected! IP = " << ipAddress << std::endl;
    return true;
}

bool ServerApp::Send(const std::string& SendData)
{
    if (NETWORK_INTERFACE::SEND_FUNC(clientSocket, SendData) == SOCKET_ERROR)
    {
        std::cout << "Send Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }

    std::cout << "Send Data! Data = " << SendData << " / " << SendData.size() << std::endl;

    return true;
}

bool ServerApp::Recv(std::string& Recv, size_t BufferSize)
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