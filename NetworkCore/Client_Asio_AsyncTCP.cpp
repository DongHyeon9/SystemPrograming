#include "Client_Asio_AsyncTCP.h"

static const boost::asio::ip::address SERVER_IP{ boost::asio::ip::make_address_v4("127.0.0.1") };

bool Client_Asio_AsyncTCP::Initialize()
{
	boost::asio::ip::tcp::endpoint endpoint{ SERVER_IP, PORT_NUMBER };
	socket.async_connect(endpoint, 
		[this](const boost::system::error_code& ErrCode) {
			if (ErrCode)
			{
				std::cout << "Connect Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
				return;
			}
			std::cout << "Connect Success!" << std::endl;
			DoSend();
		});

	ioThread = std::thread([this]() { ctx.run(); });

	std::cout << "Asio AsyncTCP Client Start!" << std::endl;

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
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Client_Asio_AsyncTCP::DoSend()
{
	buffer = "Hello Asio!";
	boost::asio::async_write(socket, boost::asio::buffer(buffer),
		[this](const boost::system::error_code& ErrCode, std::size_t BytesTransferred) {
			Send_Internal(ErrCode, BytesTransferred);
		});
}

void Client_Asio_AsyncTCP::Send_Internal(const boost::system::error_code& ErrCode, std::size_t BytesTransferred)
{
	if (ErrCode)
	{
		std::cout << "Send Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
		return;
	}

	std::cout << "Send [" << BytesTransferred << "] : " << buffer << std::endl;
	DoReceive();
}

void Client_Asio_AsyncTCP::DoReceive()
{
	boost::asio::async_read(socket, boost::asio::buffer(buffer),
		[this](const boost::system::error_code& ErrCode, std::size_t BytesTransferred) {
			Receive_Internal(ErrCode, BytesTransferred);
		});
}

void Client_Asio_AsyncTCP::Receive_Internal(const boost::system::error_code& ErrCode, std::size_t BytesTransferred)
{
	if (ErrCode)
	{
		std::cout << "Recv Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
		return;
	}

	std::cout << "Recv [" << BytesTransferred << "] : " << buffer << std::endl;
	
	std::this_thread::sleep_for(std::chrono::seconds(1));
	DoSend();
}
