#include "Server_IOCP.h"

constexpr uint64 THREAD_COUNT{ 1 };
static constexpr DWORD BUFFER_COUNT{ 1 };

namespace IO_TYPE
{
    enum : uint8
    {
        READ,
        WRITE,
    };
}

void CALLBACK SendCallback(DWORD Error, DWORD RecvLen, LPWSAOVERLAPPED Overlapped, DWORD Flags)
{
    OverlappedEx* overlapped{ reinterpret_cast<OverlappedEx*>(Overlapped) };
    std::string* buffer{ reinterpret_cast<std::string*>(overlapped->wsaBuf.buf) };
    std::cout << "Send Data! Data = " << *buffer << " / " << buffer->size() << std::endl;

    delete buffer;
    delete overlapped;
}

bool Server_IOCP::Initialize()
{
	std::cout << "Initialize Server IOCP" << std::endl;

	if (!__super::Initialize()) return false;

    // 1. 소켓 생성
    socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

    // 4.WorkerThread 생성
	workerThread.reserve(THREAD_COUNT);
	for (uint64 i = 0; i < THREAD_COUNT; ++i)
	    workerThread.emplace_back([this]() {WorkerRun(); });

    // 5. CompletionPort생성
    hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	return true;
}

void Server_IOCP::Deinitialize()
{
    bTerminate = true;
    
    // IOCP를 깨우기 위한 NULL 패킷
    for (size_t i = 0; i < workerThread.size(); ++i)
        ::PostQueuedCompletionStatus(hIOCP, 0, NULL, NULL);

    for (auto& thread : workerThread)
        thread.join();

    ::CloseHandle(hIOCP);

	__super::Deinitialize();
}

void Server_IOCP::Run()
{
    while (true)
    {
        Session* session{ new Session{} };

        int32 addrLen{ sizeof(SOCKADDR_IN) };
        session->socket = ::accept(socket, reinterpret_cast<SOCKADDR*>(&session->addr), &addrLen);
        if (session->socket == INVALID_SOCKET)
        {
            std::cout << "Accept Error Code : " << ::WSAGetLastError() << std::endl;
            delete session;
            continue;
        }

        std::cout << "Client Connected! IP = " << GetIP(session->addr) << std::endl;

        // 소켓을 CP에 등록
        ::CreateIoCompletionPort(reinterpret_cast<HANDLE>(session->socket), hIOCP, /*구분용 Key*/reinterpret_cast<ULONG_PTR>(session), 0);
        
		// 최초 READ 비동기 요청
		OverlappedEx* overlappedEX{ new OverlappedEx{} };
		overlappedEX->type = IO_TYPE::READ;
		overlappedEX->wsaBuf.buf = session->buffer.data();
		overlappedEX->wsaBuf.len = RECV_SIZE;

		DWORD flags{};
		DWORD len{};
		::WSARecv(session->socket, &overlappedEX->wsaBuf, BUFFER_COUNT, &len, &flags, &overlappedEX->overlapped, nullptr);
    }
}

void Server_IOCP::WorkerRun()
{
    while (!bTerminate)
    {
        DWORD bytesTransferred{};
        Session* session{};
        OverlappedEx* overlappedEX{};

        BOOL ret{ ::GetQueuedCompletionStatus(hIOCP,&bytesTransferred,reinterpret_cast<PULONG_PTR>(&session),reinterpret_cast<LPOVERLAPPED*>(&overlappedEX),INFINITE) };

        // 스레드 종료 조건
        if (session == nullptr || overlappedEX == nullptr)
        {
            // NULL패킷 깨우기용
            if (bTerminate) break;
            continue;
        }

        // 클라이언트 종료 감지
        if (ret == FALSE || bytesTransferred == 0)
        {
            ::closesocket(session->socket);
            delete session;
            delete overlappedEX;
            continue;
        }

        // 읽고 쓰기
        if (overlappedEX->type == IO_TYPE::READ)
        {
            std::cout << "Recv Data! Data = " << session->buffer << " / " << session->buffer.size() << std::endl;

            overlappedEX->type = IO_TYPE::WRITE;
            overlappedEX->wsaBuf.len = bytesTransferred;

            DWORD sendLen{};
            DWORD flags{};
            ::WSASend(session->socket, &overlappedEX->wsaBuf, BUFFER_COUNT, &sendLen, flags, &overlappedEX->overlapped, nullptr);
        }
        // 쓰고 읽기
        else if (overlappedEX->type == IO_TYPE::WRITE)
        {
            std::cout << "Send Data! Data = " << session->buffer << " / " << session->buffer.size() << std::endl;

            overlappedEX->type = IO_TYPE::READ;
            overlappedEX->wsaBuf.len = RECV_SIZE;
            
            DWORD recvLen{};
            DWORD flags{};
            ::WSARecv(session->socket, &overlappedEX->wsaBuf, BUFFER_COUNT, &recvLen, &flags, &overlappedEX->overlapped, nullptr);
        }
    }
}

