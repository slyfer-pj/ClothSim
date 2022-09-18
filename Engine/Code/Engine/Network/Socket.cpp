#include "Engine/Core/WinCommon.hpp"
#include "Engine/Network/Socket.hpp"

Socket::Socket()
	:m_handle(INVALID_SOCKET)
{
}

Socket::~Socket()
{
	Close();
}

void Socket::Close()
{
	if (!IsClosed())
	{
		::closesocket(m_handle);
		m_handle = INVALID_SOCKET;
	}
}

bool Socket::IsClosed() const
{
	return m_handle == INVALID_SOCKET;
}

void Socket::SetBlocking(bool isBlocking) const
{
	u_long non_blocking = isBlocking ? 0 : 1;
	::ioctlsocket(m_handle, FIONBIO, &non_blocking);
}

bool Socket::CheckForFatalError()
{
	int error = WSAGetLastError();
	switch (error)
	{
	case 0: //no error
		return false;
		break;
	case WSAEWOULDBLOCK:
		return false;
	default:
		// Log( "Disconnected due to error: %i", error )
		Close();
		return true;
	}
}
