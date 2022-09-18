#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Renderer/Window.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include "ThirdParty/ImGUI/imgui.h"
#include "ThirdParty/ImGUI/imgui_impl_dx11.h"
#include "ThirdParty/ImGUI/imgui_impl_win32.h"
#include "Game/App.hpp"
#include "Game/Game.hpp"

Renderer* g_theRenderer = nullptr;
App* g_theApp = nullptr;
InputSystem* g_theInput = nullptr;
AudioSystem* g_theAudio = nullptr;
Window* g_theWindow = nullptr;

void App::Startup()
{
	LoadGameConfigBlackboard();
	EventSystemConfig eventSystemConfig;
	g_theEventSystem = new EventSystem(eventSystemConfig);

	InputSystemConfig inputConfig;
	g_theInput = new InputSystem(inputConfig);

	WindowConfig windowConfig;
	windowConfig.m_clientAspect = g_gameConfigBlackboard.GetValue("windowAspect", windowConfig.m_clientAspect);
	windowConfig.m_inputSystem = g_theInput;
	windowConfig.m_isFullscreen = g_gameConfigBlackboard.GetValue("isFullscreen", windowConfig.m_isFullscreen);
	windowConfig.m_windowTitle = g_gameConfigBlackboard.GetValue("windowTitle", windowConfig.m_windowTitle);
	g_theWindow = new Window(windowConfig);

	RendererConfig renderConfig;
	renderConfig.m_window = g_theWindow;
	g_theRenderer = new Renderer(renderConfig);

	DevConsoleConfig consoleConfig;
	consoleConfig.m_renderer = g_theRenderer;
	consoleConfig.m_fontFilePath = g_gameConfigBlackboard.GetValue("devconsoleDefaultFont", consoleConfig.m_fontFilePath);
	consoleConfig.m_fontAspect = g_gameConfigBlackboard.GetValue("devconsoleFontAspect", consoleConfig.m_fontAspect);
	consoleConfig.m_fontCellHeight = g_gameConfigBlackboard.GetValue("devconsoleTextHeight", consoleConfig.m_fontCellHeight);
	consoleConfig.m_maxLinesToPrint = g_gameConfigBlackboard.GetValue("devconsoleMaxLines", consoleConfig.m_maxLinesToPrint);
	g_theConsole = new DevConsole(consoleConfig);

	AudioSystemConfig audioConfig;
	g_theAudio = new AudioSystem(audioConfig);

	g_theEventSystem->Startup();
	g_theInput->Startup();
	g_theWindow->Startup();
	g_theRenderer->Startup();
	g_theConsole->Startup();
	g_theAudio->Startup();

	//initialize ImGUI
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(g_theWindow->GetOSWindowHandle());
	ImGui_ImplDX11_Init(g_theRenderer->GetDevice(), g_theRenderer->GetDeviceContext());

	m_theGame = new Game();
	m_theGame->Startup();
}

void App::BeginFrame()
{	
	Clock::SystemBeginFrame();

	g_theEventSystem->BeginFrame();
	g_theInput->BeginFrame();
	g_theWindow->BeginFrame();
	g_theRenderer->BeginFrame();
	g_theConsole->BeginFrame();
	g_theAudio->BeginFrame();

	//imgui begin frame functions
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void App::Update()
{
	HandleKeyboardInput();
	float deltaSeconds = static_cast<float>(Clock::GetSystemClock().GetDeltaTime());
	m_theGame->Update(deltaSeconds);
}

void App::Render() const
{
	g_theRenderer->ClearScreen(Rgba8(127, 127, 127, 255));
	m_theGame->Render();

	//imgui render function
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void App::EndFrame()
{
	if (m_stepMode)
		m_isPaused = true;

	g_theEventSystem->EndFrame();
	g_theInput->EndFrame();
	g_theWindow->EndFrame();
	g_theRenderer->EndFrame();
	g_theConsole->EndFrame();
	g_theAudio->EndFrame();
}

void App::Shutdown()
{
	m_theGame->ShutDown();
	delete m_theGame;
	m_theGame = nullptr;

	//imgui shutdown
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	g_theAudio->Shutdown();
	delete g_theAudio;
	g_theAudio = nullptr;

	g_theConsole->Shutdown();
	delete g_theConsole;
	g_theConsole = nullptr;

	g_theRenderer->Shutdown();
	delete g_theRenderer;
	g_theRenderer = nullptr;

	g_theWindow->Shutdown();
	delete g_theWindow;
	g_theWindow = nullptr;

	g_theInput->Shutdown();
	delete g_theInput;
	g_theInput = nullptr;

	g_theEventSystem->Shutdown();
	delete g_theEventSystem;
	g_theEventSystem = nullptr;
}

void App::RunFrame()
{
	BeginFrame();
	Update();
	Render();
	EndFrame();
}

void App::Run()
{
	while (!IsQuitting())
	{
		RunFrame();
	}
}

void App::HandleKeyboardInput()
{
	if (g_theInput->WasKeyJustPressed(KEYCODE_TILDE))
	{
		g_theConsole->ToggleMode(DevConsoleMode::OPEN_FULL);
	}
	if (g_theConsole->GetMode() == DevConsoleMode::OPEN_FULL)
	{
		g_theInput->ConsumeKeyJustPressed(KEYCODE_ESCAPE);
	}
	/*if (g_theInput->WasKeyJustPressed('P'))
	{
		m_isPaused = !m_isPaused;
		m_stepMode = false;
	}
	if (g_theInput->WasKeyJustPressed('O'))
	{
		m_isPaused = false;
		m_stepMode = true;
	}
	if (g_theInput->IsKeyDown('T'))
	{
		m_isSlomo = true;
	}
	if (!g_theInput->IsKeyDown('T'))
	{
		m_isSlomo = false;
	}*/
}

void App::HandleQuitRequest()
{
	m_isQuitting = true;
}
