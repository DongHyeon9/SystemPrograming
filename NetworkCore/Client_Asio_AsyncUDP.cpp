#include "Client_Asio_AsyncUDP.h"

static const boost::asio::ip::address SERVER_IP{ boost::asio::ip::make_address_v4("127.0.0.1") };

bool Client_Asio_AsyncUDP::Initialize()
{
	boost::system::error_code errCode{};

	socket.open(boost::asio::ip::udp::v4(), errCode);
	if (errCode)
	{
		std::cout << "Socket Open Error [" << errCode.value() << "] : " << errCode.message() << std::endl;
		return false;
	}

	remoteEndpoint = boost::asio::ip::udp::endpoint(SERVER_IP, PORT_NUMBER);
	ioThread = std::thread([this]() { ctx.run(); });

	DoSend();

	std::cout << "Asio AsyncUDP Client Start!" << std::endl;

	return true;
}

void Client_Asio_AsyncUDP::Deinitialize()
{
	ctx.stop();
}

void Client_Asio_AsyncUDP::Run()
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void Client_Asio_AsyncUDP::DoSend()
{
	std::string msg{ "Hello Asio!" };
	std::memcpy(buffer.data(), msg.data(), msg.size());

	socket.async_send_to(boost::asio::buffer(buffer.data(), msg.size()), remoteEndpoint,
		[this](const boost::system::error_code& ErrCode, std::size_t BytesTransferred) {
			Send_Internal(ErrCode, BytesTransferred);
		});
}

void Client_Asio_AsyncUDP::Send_Internal(const boost::system::error_code& ErrCode, std::size_t BytesTransferred)
{
	if(ErrCode)
	{
		std::cout << "Send Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
		return;
	}

	std::string sendMsg{ buffer.data(), BytesTransferred };
	std::cout << "Send [" << BytesTransferred << "] : " << sendMsg << std::endl;
	DoReceive();
}

void Client_Asio_AsyncUDP::DoReceive()
{
	socket.async_receive_from(
		boost::asio::buffer(buffer.data(), buffer.size()), remoteEndpoint,
		[this](const boost::system::error_code& ErrCode, std::size_t BytesTransferred) {
			Receive_Internal(ErrCode, BytesTransferred);
		});
}

void Client_Asio_AsyncUDP::Receive_Internal(const boost::system::error_code& ErrCode, std::size_t BytesTransferred)
{
	if(ErrCode)
	{
		std::cout << "Recv Error [" << ErrCode.value() << "] : " << ErrCode.message() << std::endl;
		return;
	}

	std::string recvMsg{ buffer.data(), BytesTransferred };
	std::cout << "Recv [" << BytesTransferred << "] : " << recvMsg << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	DoSend();
}