#include "Engine/Core/WinCommon.hpp"
#include "Engine/Network/TCPConnection.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Network/NetAddress.hpp"
//#include "Engine/Core/ErrorWarningAssert.hpp"

bool TCPConnection::Connect(const NetAddress& address)
{
	sockaddr_in ipv4;

	//unsigned int uintAddr = 0x0A09A34D;		//127.0.0.1 => 0x7f000001
	//uint16_t port = 3121;

	ipv4.sin_family = AF_INET;		//identifies as sockaddr_in
	ipv4.sin_addr.S_un.S_addr = ::htonl(address.address);
	ipv4.sin_port = ::htons(address.port);

	//create our socket
	SOCKET mySocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);		//creating TCP over ipv4
	if (mySocket == INVALID_SOCKET)
	{
		ERROR_AND_DIE("Unable to create a socket");
	}

	//will block until failure or success
	int result = ::connect(mySocket, (sockaddr*)(&ipv4), static_cast<int>(sizeof(ipv4)));
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		::closesocket(mySocket);
		return false;
	}

	m_handle = mySocket;
	m_address = address;
	return true;
}

size_t TCPConnection::Send(const void* data, const size_t dataSize)
{
	if (IsClosed())
	{
		return 0;
	}

	int byteSent = ::send(m_handle, static_cast<const char*>(data), static_cast<int>(dataSize), 0);
	if (byteSent > 0)
	{
		GUARANTEE_OR_DIE(byteSent == dataSize, "Something went wrong while sending data to remote");
		return byteSent;
	}
	else if (byteSent == 0)
	{
		//special case: socket was closed cleanly
		Close();
		return 0;
	}
	else
	{
		//socket error
		Close();
		return 0;
	}
}

size_t TCPConnection::Receive(void* buffer, size_t maxBytesToRead)
{
	int bytesRead = ::recv(m_handle, static_cast<char*>(buffer), static_cast<int>(maxBytesToRead), 0);
	if (bytesRead == 0) 
	{
		Close(); // clean close
		return 0;
	}
	else if (bytesRead > 0) 
	{
		return static_cast<size_t>(bytesRead);
	}
	else 
	{
		// error, handle it
		//Close();
		CheckForFatalError();
		return 0;
	}
}

