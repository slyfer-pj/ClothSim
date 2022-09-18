#include "Engine/Core/WinCommon.hpp"
#include "Engine/Network/RemoteConsole.hpp"
#include "Engine/Network/TCPConnection.hpp"
#include "Engine/Network/NetAddress.hpp"
#include "Engine/Network/TCPServer.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/VertexUtils.hpp"

constexpr uint16_t SERVICE_PORT = 3121;

struct rdc_header_t
{
	uint16_t payload_size;
};

#pragma pack(push, 1)
struct rdc_payload_t
{
	char is_echo; // 0 cmd, 1 echo... other values... user defined?
	uint16_t message_size; // including null
	// char const* message; 
};
#pragma pack(pop)

RemoteConsole::RemoteConsole(const RemoteConsoleConfig& config)
	:m_devConsole(config.m_console)
{
}

void RemoteConsole::Startup()
{
	AttemptToHost(SERVICE_PORT, true);

	/*if (m_server) {
		std::vector<NetAddress> addresses = NetAddress::GetAllInternal(3121);

		for (int i = 0; i < addresses.size(); ++i) {
			DebuggerPrintf(addresses[i].ToString().c_str());
			DebuggerPrintf("\n");
		}
	}*/

	//TCPConnection* con = new TCPConnection();
	//NetAddress address = NetAddress::FromString("127.0.0.1:3121");
	//if (con->Connect(address))
	//{
	//	con->SetBlocking(false);
	//	con->Send("Hello World", 12);
	//	//size_t 

	//	m_connections.push_back(con);
	//}
	//else
	//{
	//	delete con;
	//}

	//uint16_t servicePort = 3121; //0x0c31
	//m_server = new TCPServer();
	//if (m_server->Host(servicePort))
	//{
	//	DebuggerPrintf("Hosting at address: %d\n", m_server->m_address.address);
	//}
}

void RemoteConsole::Shutdown()
{

}

void RemoteConsole::Update()
{
	if (m_connectionState == ConnectionState::HOSTING)
	{
		TCPConnection* incoming = m_server->Accept();
		if (incoming)
		{
			ConnectionData data;
			data.m_connection = incoming;
			m_connections.push_back(data);
		}
	}

	ProcessConnections();
}

void RemoteConsole::Render(Renderer& renderer, const AABB2& bounds, BitmapFont& font)
{
	constexpr float cellHeight = 20.f;
	const Vec2 boxDimensions((bounds.m_maxs.x - bounds.m_mins.x) / 2.f, (bounds.m_maxs.y - bounds.m_mins.y) / 2.f);
	Vec2 boxMins = Vec2(boxDimensions.x / 2.f, bounds.m_maxs.y - boxDimensions.y);
	AABB2 boxBounds = AABB2(boxMins, boxMins + boxDimensions);

	std::vector<Vertex_PCU> bgBox;
	AddVertsForAABB2D(bgBox, boxBounds, Rgba8(0, 0, 255, 127));
	renderer.BindTexture(nullptr);
	renderer.DrawVertexArray((int)bgBox.size(), bgBox.data());

	std::vector<Vertex_PCU> textVerts;
	std::string consoleData;
	consoleData.append(GetCurrentStateString() + ": ");
	if (m_connectionState == ConnectionState::HOSTING)
	{
		consoleData.append(m_serverExternalAddess);
	}
	consoleData.append("\n");

	for (int i = 0; i < m_connections.size(); i++)
	{
		consoleData.append(Stringf("%d: %s\n", i, m_connections[i].m_connection->m_address.ToString().c_str()));
	}

	font.AddVertsForTextInBox2D(textVerts, boxBounds, cellHeight, consoleData, Rgba8::WHITE, 1.f, Vec2(0.f, 1.f));
	renderer.BindTexture(&font.GetTexture());
	renderer.DrawVertexArray((int)textVerts.size(), textVerts.data());
}

void RemoteConsole::ProcessConnections()
{
	for (int i = 0; i < m_connections.size(); i++)
	{
		ConnectionData conn = m_connections[i];
		/*size_t recvd = cp->Receive(buffer, sizeof(buffer));
		if (recvd > 0)
		{
			buffer[recvd] = NULL;
			m_devConsole->AddLine(m_devConsole->INFO_MAJOR, Stringf("size = %d data = %s", recvd, &buffer));
		}*/
		if (conn.m_bytesReceived < 2) 
		{
			conn.m_bytesReceived += conn.m_connection->Receive(conn.m_buffer + conn.m_bytesReceived, 2 - conn.m_bytesReceived);
		}
		if (conn.m_bytesReceived >= 2) 
		{
			uint16_t payloadSize = ::ntohs(*(uint16_t*)conn.m_buffer);
			size_t recvSize = payloadSize - (conn.m_bytesReceived - 2); // 
			conn.m_bytesReceived += conn.m_connection->Receive(conn.m_buffer + conn.m_bytesReceived, recvSize);
			if (conn.m_bytesReceived >= (payloadSize + 2)) 
			{
				conn.m_buffer[conn.m_bytesReceived] = NULL;
				m_devConsole->AddLine(m_devConsole->INFO_MAJOR, Stringf("size = %d data = %s", conn.m_bytesReceived, &conn.m_buffer));
			}
		}

		if (conn.m_connection->IsClosed()) 
		{
			delete conn.m_connection;
			m_connections.erase(m_connections.begin() + i);
			--i;
		}
	}
}

void RemoteConsole::AttemptToHost(uint16_t port, bool loopbackIfFailed)
{
	m_server = new TCPServer();
	if (m_server->Host(port))
	{
		CloseAllConnections();
		m_server->SetBlocking(false);
		std::vector<NetAddress> addresses = m_server->m_address.GetAllInternal(port);
		m_serverExternalAddess = addresses[0].ToString();
		m_devConsole->AddLine(m_devConsole->INFO_MINOR, Stringf("Hosting at %s", m_serverExternalAddess.c_str()));
		m_connectionState = ConnectionState::HOSTING;
	}
	else
	{
		m_devConsole->AddLine(m_devConsole->ERRORTEXT, Stringf("Failed to host at port %d", port));
		if (loopbackIfFailed)
		{
			//try to join local host
			NetAddress loopback = NetAddress::GetLoopBack(port);
			ConnectToHost(loopback);
		}
	}
}

void RemoteConsole::ConnectToHost(NetAddress hostAddress)
{
	TCPConnection* conToHost = new TCPConnection();
	if (conToHost->Connect(hostAddress))
	{
		conToHost->SetBlocking(false);
		ConnectionData data;
		data.m_connection = conToHost;
		m_connections.push_back(data);
		m_connectionState = ConnectionState::CLIENT;
		m_devConsole->AddLine(m_devConsole->INFO_MINOR, Stringf("Connected as client to %s", conToHost->m_address.ToString().c_str()));
		//send some initial data to the host after connecting.
		conToHost->Send("Hello", 6);
	}
	else
	{
		delete conToHost;
		m_devConsole->AddLine(m_devConsole->ERRORTEXT, Stringf("Failed to connect to %s", hostAddress.ToString().c_str()));
		m_connectionState = ConnectionState::DISCONNECTED;
	}
}

void RemoteConsole::SendCommand(int connIdx, const std::string& cmd, bool isEcho)
{
	TCPConnection* cp = m_connections[connIdx].m_connection; // add error checking

	//---------------------------------------------------------
   // manual - method 1
	//char buffer[512];
	size_t payloadSize = 1 // isEcho
		+ 2 // cmd size with null
		+ cmd.size() + 1; // cmd

	//size_t packetSize = 2  // size of payload
	//	+ payloadSize; // actual payload

	/*char* iter = buffer;

	*((uint16_t*)iter) = ::htons(payloadSize);
	iter += sizeof(uint16_t);

	*iter = 0;
	++iter;

	*((uint16_t*)iter) = ::htons((uint16_t)(cmd.size() + 1));
	iter += sizeof(uint16_t);

	::memcpy(iter, cmd.c_str(), cmd.size() + 1);

	cp->Send(buffer, packetSize);*/

	//---------------------------------------------------------
	// struct helper - method 2
	rdc_header_t header;
	header.payload_size = ::htons(payloadSize);

	rdc_payload_t payload;
	payload.is_echo = isEcho;
	payload.message_size = ::htons((uint16_t)(cmd.size() + 1));

	cp->Send(&header, sizeof(header));
	cp->Send(&payload, sizeof(payload));
	cp->Send(cmd.c_str(), cmd.size() + 1);

	//---------------------------------------------------------
}

void RemoteConsole::CloseAllConnections()
{
	for (int i = 0; i < m_connections.size(); i++)
	{
		m_connections[i].m_connection->Close();
		delete m_connections[i].m_connection;
		m_connections[i].m_connection = nullptr;
	}

	m_connections.clear();
	m_connectionState = ConnectionState::DISCONNECTED;
}

void RemoteConsole::StopHosting()
{
	if (m_connectionState == ConnectionState::HOSTING)
	{
		CloseAllConnections();
		m_server->Close();
		delete m_server;
		m_server = nullptr;
		m_connectionState = ConnectionState::DISCONNECTED;
	}
}

std::string RemoteConsole::GetCurrentStateString() const
{
	switch (m_connectionState)
	{
	case ConnectionState::DISCONNECTED:
		return "Disconnected";
	case ConnectionState::HOSTING:
		return "Hosting";
	case ConnectionState::CLIENT:
		return "Client";
	default:
		return "Error";
	}
}

