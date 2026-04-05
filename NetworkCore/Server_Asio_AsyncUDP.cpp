#include "Server_Asio_AsyncUDP.h"

bool Server_Asio_AsyncUDP::Initialize()
{
	endpoint.address(boost::asio::ip::address_v4::any());
	endpoint.port(PORT_NUMBER);

	boost::system::error_code errCode{};

	socket.open(boost::asio::ip::udp::v4(), errCode);

	if(errCode)
	{
		std::cout << "Socket Open Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return false;
	}

	socket.bind(endpoint, errCode);

	if (errCode)
	{
		std::cout << "Socket Bind Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return false;
	}

	session = std::make_shared<Session_Asio<E_PROTOCOL_TYPE::UDP>>(std::move(socket));
	session->Start();

	ioThread = std::thread([this]() { ctx.run(); });

	std::cout << "Asio AsyncUDP Server Start!" << std::endl;

	return true;
}

void Server_Asio_AsyncUDP::Deinitialize()
{
	ctx.stop();
}

void Server_Asio_AsyncUDP::Run()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}