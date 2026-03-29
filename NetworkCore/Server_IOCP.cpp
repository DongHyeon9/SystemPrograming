#include "Server_IOCP.h"

constexpr uint64 THREAD_COUNT{ 4 };
static constexpr DWORD BUFFER_COUNT{ 1 };

static LPFN_CONNECTEX ConnectEx{};
static LPFN_DISCONNECTEX DisconnectEx{};
static LPFN_ACCEPTEX Accept_ex{};

namespace IO_TYPE
{
    enum : uint8
    {
        READ,
        WRITE,
    };
}

bool Server_IOCP::Initialize()
{
	std::cout << "Initialize Server IOCP" << std::endl;

	if (!__super::Initialize()) return false;

    // 1. 소켓 생성
    // WSASocket : socket에서 인자를 더 세분화할 수 있는 버전
    //socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (socket == INVALID_SOCKET)
    {
        std::cout << "Socket Error Code : " << ::WSAGetLastError() << std::endl;
        return false;
    }
    
    // ::WSAIoctl
    // 필요에 따라 소켓 고급 기능을 사용할 수 있다
    // https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsaioctl
    // ex) SIO_GET_EXTENSION_FUNCTION_POINTER 명령으로 AcceptEx, ConnectEx 등의 함수 포인터를 가져올 수 있다.

    {
		// AcceptEx 함수 포인터 가져오기
        // 사용하는 이유
        // https://stackoverflow.com/questions/4470645/acceptex-without-wsaioctl
        GUID guid = WSAID_ACCEPTEX;
        LPVOID* fn{ reinterpret_cast<LPVOID*>(&Accept_ex) };
        DWORD bytes{};
        if (SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL))
        {
            std::cout << "WSAIoctl Error Code : " << ::WSAGetLastError() << std::endl;
            return false;
        }
    }

    {
        // DisconnectEx 함수 포인터 가져오기
        GUID guid = WSAID_DISCONNECTEX;
        LPVOID* fn{ reinterpret_cast<LPVOID*>(&DisconnectEx) };
        DWORD bytes{};
        if (SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL))
        {
            std::cout << "WSAIoctl Error Code : " << ::WSAGetLastError() << std::endl;
            return false;
        }
    }

    {
        // DisconnectEx 함수 포인터 가져오기
        GUID guid = WSAID_CONNECTEX;
        LPVOID* fn{ reinterpret_cast<LPVOID*>(&ConnectEx) };
        DWORD bytes{};
        if (SOCKET_ERROR != ::WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT & bytes, NULL, NULL))
        {
            std::cout << "WSAIoctl Error Code : " << ::WSAGetLastError() << std::endl;
            return false;
        }
    }


	// ::setsockopt
    // 소켓의 옵션을 설정하는 함수
    // https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-setsockopt
    // ex)
    //  - SO_REUSEADDR : 바인딩 전에 IP주소와 포트를 재사용
    //  - TCP_NODELAY : Nagle 알고리즘 비활성화
	//  - SO_RCVBUF : 수신버퍼(운영체제) 사이즈 설정
    //  - SO_SNDBUF : 송신버퍼(운영체제) 사이즈 설정

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

	::closesocket(socket);
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

