#include "Client_Asio_SyncUDP.h"

static const boost::asio::ip::address SERVER_IP{ boost::asio::ip::make_address_v4("127.0.0.1") };

bool Client_Asio_SyncUDP::Initialize()
{
    std::cout << "Asio SyncUDP Client Start!" << std::endl;

    return true;
}

void Client_Asio_SyncUDP::Deinitialize()
{
}

void Client_Asio_SyncUDP::Run()
{
    boost::system::error_code errCode{};
    boost::asio::ip::udp::endpoint localEndpoint{ boost::asio::ip::udp::v4(), 0 };
    boost::asio::ip::udp::socket socket{ ctx, localEndpoint };
    
    boost::asio::ip::udp::endpoint remoteEndpoint{ SERVER_IP, PORT_NUMBER };

    while (true)
    {
        std::string buffer{ "Hello Asio!" };
        
        const std::size_t sendSize{ socket.send_to(boost::asio::buffer(buffer.data(), buffer.size()), remoteEndpoint, 0, errCode) };
        if (errCode)
        {
            std::cout << "Send Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
            socket.close();
            return;
        }
        std::cout << "Send [" << sendSize << "] : " << buffer << std::endl;
        
        boost::asio::ip::udp::endpoint recvEndpoint{};
        std::string recvBuffer{};
        recvBuffer.resize(BUFFER_SIZE);
        
        const std::size_t recvSize{ socket.receive_from(boost::asio::buffer(recvBuffer.data(), BUFFER_SIZE), recvEndpoint, 0, errCode) };
        if (errCode)
        {
            std::cout << "Recv Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
            socket.close();
            return;
        }
        recvBuffer.resize(recvSize);
        std::cout << "Recv [" << recvSize << "] : " << recvBuffer << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	socket.close();
}
