#pragma once
#include "Engine/Network/TCPSocket.hpp"

class TCPConnection;

class TCPServer : public TCPSocket
{
public:
	bool Host(uint16_t service, unsigned int backlog = 16);		//listen on a socket for new connection
	TCPConnection* Accept();
};