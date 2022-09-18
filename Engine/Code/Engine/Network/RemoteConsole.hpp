#pragma once
#include "Engine/Network/NetAddress.hpp"
#include <vector>
#include <string>

class DevConsole;
class Renderer;
class TCPServer;
class TCPConnection;
struct AABB2;
class BitmapFont;

struct ConnectionData
{
	TCPConnection* m_connection = nullptr;
	char m_buffer[1024];
	size_t m_bytesReceived = 0;
};

enum class ConnectionState
{
	DISCONNECTED,
	HOSTING,
	CLIENT
};

struct RemoteConsoleConfig
{
	DevConsole* m_console = nullptr;
};

class RemoteConsole
{
public:
	RemoteConsole(const RemoteConsoleConfig& config);

	void Startup();
	void Shutdown();

	void Update();
	void Render(Renderer& renderer, const AABB2& bounds, BitmapFont& font);
	void ProcessConnections();
	void AttemptToHost(uint16_t port, bool loopbackIfFailed);
	void ConnectToHost(NetAddress hostAddress);
	void SendCommand(int connIdx, const std::string& cmd, bool isEcho);
	void CloseAllConnections();
	void StopHosting();

private:
	ConnectionState m_connectionState = ConnectionState::DISCONNECTED;
	DevConsole* m_devConsole = nullptr;
	TCPServer* m_server = nullptr;
	std::string m_serverExternalAddess;
	std::vector<ConnectionData> m_connections = {};

private:
	std::string GetCurrentStateString() const;
};