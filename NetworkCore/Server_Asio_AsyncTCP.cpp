#include "Server_Asio_AsyncTCP.h"

bool Server_Asio_AsyncTCP::Initialize()
{
	endpoint.port(PORT_NUMBER);
	endpoint.address(boost::asio::ip::address_v4::any());
	boost::system::error_code errCode{};
	
	acceptor.open(boost::asio::ip::tcp::v4(), errCode);
	if (errCode)
	{
		std::cout << "Acceptor Open Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return false;
	}

	acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), errCode);
	if (errCode)
	{
		std::cout << "Acceptor Set Option Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return false;
	}

	acceptor.bind(endpoint, errCode);
	if (errCode)
	{
		std::cout << "Acceptor Bind Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return false;
	}

	acceptor.listen(boost::asio::socket_base::max_listen_connections, errCode);
	if (errCode)
	{
		std::cout << "Acceptor Listen Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return false;
	}

	ioThread = std::thread([this]() { ctx.run(); });
	DoAccept();

	std::cout << "Asio AsyncTCP Server Start!" << std::endl;

	return true;
}

void Server_Asio_AsyncTCP::Deinitialize()
{
	acceptor.close();
	ctx.stop();
	if (ioThread.joinable())
	{
		ioThread.join();
	}
}

void Server_Asio_AsyncTCP::Run()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Server_Asio_AsyncTCP::DoAccept()
{
	acceptor.async_accept([this](boost::system::error_code ErrCode, boost::asio::ip::tcp::socket Socket) {
		if (ErrCode)
		{
			std::cout << "Accept Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
			return;
		}

		std::cout << "Accept Client!" << std::endl;

		auto session = std::make_shared<Session_Asio>(std::move(Socket));
		sessionList.emplace_back(session);
		session->Start();

		DoAccept();
		});
}
