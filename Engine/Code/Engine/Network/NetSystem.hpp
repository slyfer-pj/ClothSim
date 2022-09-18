#pragma once

struct NetConfig
{

};

class NetSystem
{
public:
	NetSystem(const NetConfig& config);
	~NetSystem();

	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	static bool Exists();

private:
	NetConfig m_config;

};