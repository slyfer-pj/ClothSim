#pragma once
#include "Engine/Math/Vec2.hpp"
#include <string>

class InputSystem;

struct WindowConfig
{
	InputSystem* m_inputSystem = nullptr;
	std::string m_windowTitle = "Untitled App";
	float m_clientAspect = 2.0f;
	bool m_isFullscreen = false;
};

class Window
{
public:
	Window(const WindowConfig& config);
	~Window();

	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	const WindowConfig& GetConfig() const;
	static Window* GetWindowContext();
	void* GetOSWindowHandle() const { return m_osWindowHandle; }
	IntVec2 GetClientDimensions() const { return m_clientDimensions; }
	Vec2 GetNormalizedCursorPos() const;
	bool HasFocus();

protected:
	void CreateOSWindow();
	void RunMessagePump();

protected:
	WindowConfig m_config;
	static Window* s_mainWindow;
	void* m_osWindowHandle = nullptr;
	IntVec2 m_clientDimensions = IntVec2::ONE;
};