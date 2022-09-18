#include "Engine/Network/NetAddress.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <WS2tcpip.h>

NetAddress NetAddress::FromString(const std::string& addressString)
{
	Strings tokens = SplitStringOnDelimiter(addressString, ':');

	IN_ADDR addr;
	int result = ::inet_pton(AF_INET, tokens[0].c_str(), &addr);
	if (result == SOCKET_ERROR)
	{
		return NetAddress();
	}

	uint16_t port = static_cast<uint16_t>(::atoi(tokens[1].c_str()));

	NetAddress address;
	address.address = ::ntohl(addr.S_un.S_addr);
	address.port = port;
	return address;
}

NetAddress NetAddress::GetLoopBack(uint16_t port)
{
	NetAddress addr;
	addr.address = 0x7f000001; // 127 0 0 1
	addr.port = port;
	return addr;
}

// we want print an address other can join
// hosting 0.0.0.0 (INADDR_ANY)
std::vector<NetAddress> NetAddress::GetAllInternal(uint16_t port)
{
	std::vector<NetAddress> results;

	char hostName[256];
	int result = ::gethostname(hostName, sizeof(hostName));
	if (result == SOCKET_ERROR)
	{
		return results;
	}

	addrinfo hints;
	::memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;	//only get ipv4 addrs
	hints.ai_socktype = SOCK_STREAM; //only tcp addresses

	addrinfo* addresses = nullptr;
	int status = ::getaddrinfo(hostName, nullptr, &hints, &addresses);

	if (status != 0)
	{
		return results;
	}

	addrinfo* iter = addresses;
	while (iter != nullptr)
	{
		NetAddress addr;
		sockaddr_in* ipv4 = (sockaddr_in*)iter->ai_addr;
		addr.address = ::ntohl(ipv4->sin_addr.S_un.S_addr);
		addr.port = port;
		results.push_back(addr);

		iter = iter->ai_next;
	}

	::freeaddrinfo(addresses);

	return results;
}

std::string NetAddress::ToString()
{
	return Stringf("%u.%u.%u.%u:%u",
		(address & 0xff000000) >> 24,
		(address & 0x00ff0000) >> 16,
		(address & 0x0000ff00) >> 8,
		(address & 0x000000ff) >> 0,
		port
	);
}
