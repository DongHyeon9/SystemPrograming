#include "Functions.h"

SOCKET __stdcall CreateSocket()
{
    return ::socket(AF_INET, SOCK_DGRAM, 0);
}

int32 __stdcall Connect(SOCKET Socket, const SOCKADDR_IN& ServerAddress)
{
    return ::connect(Socket, reinterpret_cast<const SOCKADDR*>(&ServerAddress), sizeof(ServerAddress));
}

int32 __stdcall Bind(SOCKET Socket, const SOCKADDR_IN& ServerAddress)
{
    return ::bind(Socket, reinterpret_cast<const SOCKADDR*>(&ServerAddress), sizeof(ServerAddress));
}

SOCKET __stdcall Accept(SOCKET ServerSocket, SOCKADDR_IN& ClientAddr, int32& AddrLen)
{
    return ::accept(ServerSocket, reinterpret_cast<SOCKADDR*>(&ClientAddr), &AddrLen);
}

int32 __stdcall Send(SOCKET Socket, const std::string& SendData)
{
    return ::sendto(Socket, SendData.c_str(), static_cast<int32>(SendData.size()), 0);
}

int32 __stdcall Recv(SOCKET Socket, std::string& RecvBuffer, size_t BufferSize)
{
    return ::recvfrom(Socket, &RecvBuffer[0], static_cast<int>(BufferSize), 0);
}