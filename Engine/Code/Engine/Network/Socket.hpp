#pragma once
#include <stdint.h>
#include "Engine/Network/NetAddress.hpp"

typedef uintptr_t SocketHandle;

class Socket
{
public:
	Socket();
	~Socket();
	void Close();

	bool IsClosed() const;
	void SetBlocking(bool isBlocking) const;
	bool CheckForFatalError();

public:
	SocketHandle m_handle;
	NetAddress m_address;
};