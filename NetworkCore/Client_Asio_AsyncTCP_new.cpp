#include "Client_Asio_AsyncTCP.h"

static const boost::asio::ip::address SERVER_IP{ boost::asio::ip::make_address_v4("127.0.0.1") };

bool Client_Asio_AsyncTCP::Initialize()
{
	std::cout << "Asio AsyncTCP Client Start!" << std::endl;

	boost::asio::ip::tcp::endpoint endpoint{ SERVER_IP, PORT_NUMBER };
	socket.async_connect(endpoint, 
		[this](const boost::system::error_code& errCode) {
			if (errCode)
			{
				std::cout << "Connect Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
				return;
			}
			std::cout << "Connect Success!" << std::endl;
			DoSend();
		});

	ioThread = std::thread([this]() { ctx.run(); });

	return true;
}

void Client_Asio_AsyncTCP::Deinitialize()
{
	socket.close();
	ctx.stop();
	if (ioThread.joinable())
	{
		ioThread.join();
	}
}

void Client_Asio_AsyncTCP::Run()
{
	while (sendCount < 10)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Client_Asio_AsyncTCP::DoSend()
{
	if (sendCount >= 10)
	{
		return;
	}

	std::string buffer{ "Hello Asio!" };
	boost::asio::async_write(socket, boost::asio::buffer(buffer),
		[this](const boost::system::error_code& errCode, std::size_t bytesTransferred) {
			HandleSend(errCode, bytesTransferred);
		});
}

void Client_Asio_AsyncTCP::HandleSend(const boost::system::error_code& errCode, std::size_t bytesTransferred)
{
	if (errCode)
	{
		std::cout << "Send Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return;
	}

	std::string sendData{ "Hello Asio!" };
	std::cout << "Send [" << bytesTransferred << "] : " << sendData << std::endl;
	DoReceive();
}

void Client_Asio_AsyncTCP::DoReceive()
{
	boost::asio::async_read(socket, boost::asio::buffer(receiveBuffer),
		[this](const boost::system::error_code& errCode, std::size_t bytesTransferred) {
			HandleReceive(errCode, bytesTransferred);
		});
}

void Client_Asio_AsyncTCP::HandleReceive(const boost::system::error_code& errCode, std::size_t bytesTransferred)
{
	if (errCode)
	{
		std::cout << "Recv Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return;
	}

	std::string recvData{ receiveBuffer.data(), bytesTransferred };
	std::cout << "Recv [" << bytesTransferred << "] : " << recvData << std::endl;
	
	++sendCount;
	if (sendCount < 10)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		DoSend();
	}
}
