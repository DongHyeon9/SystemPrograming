#include "Client_Asio_SyncTCP.h"

static const boost::asio::ip::address SERVER_IP{ boost::asio::ip::make_address_v4("127.0.0.1") };

bool Client_Asio_SyncTCP::Initialize()
{
    boost::asio::ip::tcp::endpoint endpoint{ SERVER_IP, PORT_NUMBER };
    boost::system::error_code errCode{};
    socket.connect(endpoint, errCode);

    if (errCode)
    {
        std::cout << "Connect Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
        return false;
    }

    std::cout << "Asio SyncTCP Client Start!" << std::endl;

    return true;
}

void Client_Asio_SyncTCP::Deinitialize()
{
    socket.close();
}

void Client_Asio_SyncTCP::Run()
{
    while (true)
    {
        std::string buffer{ "Hello Asio!" };

        boost::system::error_code errCode{};
        const std::size_t sendSize{ socket.write_some(boost::asio::buffer(buffer.data(), buffer.size()), errCode) };

        if (errCode)
        {
            std::cout << "Send Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
            
            return;
        }

        std::cout << "Send [" << sendSize << "] : " << buffer << std::endl;

        const std::size_t recvSize{ socket.read_some(boost::asio::buffer(buffer.data(), BUFFER_SIZE), errCode) };

        if (errCode)
        {
            std::cout << "Recv Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
            return;
        }

        std::cout << "Recv [" << recvSize << "] : " << buffer << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
