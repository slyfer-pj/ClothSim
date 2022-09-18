#include "Engine/Core/WinCommon.hpp"
#include "Engine/Network/TCPServer.hpp"
#include "Engine/Network/NetAddress.hpp"
#include "Engine/Network/TCPConnection.hpp"

bool TCPServer::Host(uint16_t service, unsigned int backlog /*= 16*/)
{
	NetAddress hostingAddess;
	hostingAddess.address = INADDR_ANY;		//0 - any IP that refers to my machine
	hostingAddess.port = service;

	//create socket
	SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket == INVALID_SOCKET)
	{
		return false;
	}

	//bind a socket to a port
	sockaddr_in ipv4;
	ipv4.sin_family = AF_INET;
	ipv4.sin_addr.S_un.S_addr = ::htonl(hostingAddess.address);
	ipv4.sin_port = ::htons(hostingAddess.port);

	int result = ::bind(socket, (sockaddr*)&ipv4, static_cast<int>(sizeof(ipv4)));
	if (result == SOCKET_ERROR)
	{
		::closesocket(socket);
		return false;
	}

	m_handle = socket;
	m_address = hostingAddess;

	result = ::listen(socket, backlog);
	if (result == SOCKET_ERROR)
	{
		Close();
		return false;
	}

	return true;
}

TCPConnection* TCPServer::Accept()
{
	if (IsClosed())
	{
		return nullptr;
	}

	sockaddr_storage addr;
	int addrLen = sizeof(addr);

	SocketHandle handle = ::accept(m_handle, (sockaddr*)&addr, &addrLen);
	if (handle == INVALID_SOCKET)
	{
		return nullptr;
	}

	if (addr.ss_family != AF_INET)
	{
		::closesocket(handle);
		return nullptr;
	}

	sockaddr_in* ipv4 = (sockaddr_in*)&addr;
	NetAddress incomingAddress;
	incomingAddress.address = ::ntohl(ipv4->sin_addr.S_un.S_addr);
	incomingAddress.port = ::ntohs(ipv4->sin_port);

	TCPConnection* incomingConn = new TCPConnection();
	incomingConn->m_handle = handle;
	incomingConn->m_address = incomingAddress;

	return incomingConn;
}

