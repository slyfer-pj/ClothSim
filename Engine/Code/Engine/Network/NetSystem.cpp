#include "Engine/Core/WinCommon.hpp"
#include "Engine/Network/NetSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

NetSystem::NetSystem(const NetConfig& config)
	:m_config(config)
{
}

NetSystem::~NetSystem()
{

}

void NetSystem::Startup()
{
	WORD version = MAKEWORD(2, 2);
	WSADATA data;

	int error = ::WSAStartup(version, &data);
	GUARANTEE_OR_DIE(error == 0, "Failed to initialize.");
}

void NetSystem::Shutdown()
{
	::WSACleanup();
}

void NetSystem::BeginFrame()
{

}

void NetSystem::EndFrame()
{

}

bool NetSystem::Exists()
{
	SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket == INVALID_SOCKET)
		return false;

	::closesocket(socket);
	return true;
}
