#include "Server_Asio_SyncTCP.h"

bool Server_Asio_SyncTCP::Initialize()
{
	endpoint.port(PORT_NUMBER);
	
	std::cout << "Asio SyncTCP Server Start!" << std::endl;

    return true;
}

void Server_Asio_SyncTCP::Deinitialize()
{
	socket.close();
}

void Server_Asio_SyncTCP::Run()
{
	boost::asio::ip::tcp::acceptor acceptor{ ctx, endpoint };

	boost::system::error_code errCode{};
	acceptor.accept(socket, errCode);

	if (errCode)
	{
		std::cout << "Accept Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return;
	}

	std::cout << "Client Accept!" << std::endl;

	while (true)
	{
		std::string buffer{};
		buffer.resize(BUFFER_SIZE, 0);
		const std::size_t recvSize{ socket.read_some(boost::asio::buffer(buffer.data(), BUFFER_SIZE), errCode) };

		if (errCode)
		{
			std::cout << "Recv Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
			return;
		}

		std::cout << "Recv [" << recvSize << "] : " << buffer << std::endl;
		
		const std::size_t sendSize{ socket.write_some(boost::asio::buffer(buffer.data(), recvSize), errCode) };

		if (errCode)
		{
			std::cout << "Send Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
			return;
		}

		std::cout << "Send [" << sendSize << "] : " << buffer << std::endl;
	}
}
