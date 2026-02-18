#include "Functions.h"

std::string GetIP(const SOCKADDR_IN& Addr)
{
	std::string ipAddress(IP_ADDR_SIZE, 0);
	::inet_ntop(AF_INET, &Addr.sin_addr, ipAddress.data(), IP_ADDR_SIZE);
	return ipAddress;
}
