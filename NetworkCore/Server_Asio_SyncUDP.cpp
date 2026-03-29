#include "Server_Asio_SyncUDP.h"

bool Server_Asio_SyncUDP::Initialize()
{
	std::cout << "Asio SyncUDP Server Start!" << std::endl;

	return true;
}

void Server_Asio_SyncUDP::Deinitialize()
{
	
}

void Server_Asio_SyncUDP::Run()
{
	boost::asio::ip::udp::socket socket{ ctx, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), PORT_NUMBER) };
	boost::system::error_code errCode{};

	while (true)
	{
		std::string buffer{};
		buffer.resize(BUFFER_SIZE);

		boost::asio::ip::udp::endpoint remoteEndpoint{};

		const std::size_t recvSize{ socket.receive_from(boost::asio::buffer(buffer.data(), BUFFER_SIZE), remoteEndpoint, 0, errCode) };

		if (errCode)
		{
			std::cout << "Recv Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
			socket.close();
			return;
		}

		buffer.resize(recvSize);
		std::cout << "Recv [" << recvSize << "] : " << buffer << std::endl;

		const std::size_t sendSize{ socket.send_to(boost::asio::buffer(buffer.data(), recvSize), remoteEndpoint, 0, errCode) };

		if (errCode)
		{
			std::cout << "Send Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
			socket.close();
			return;
		}

		std::cout << "Send [" << sendSize << "] : " << buffer << std::endl;
	}

	socket.close();
}
