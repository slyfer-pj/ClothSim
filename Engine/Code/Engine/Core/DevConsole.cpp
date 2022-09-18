#include "Engine/Core/DevConsole.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/DebugRenderSystem.hpp"
#include "Engine/Network/RemoteConsole.hpp"
#include "Engine/Network/NetSystem.hpp"
#include <string>

DevConsole* g_theConsole = nullptr;
extern InputSystem* g_theInput;

constexpr float caretLineThickness = 1.5f;
constexpr float caretBlinkTime = 0.5f;

DevConsole::DevConsole(const DevConsoleConfig& config)
	:m_config(config)
{
}

DevConsole::~DevConsole()
{

}

void DevConsole::Startup()
{
	ERRORTEXT = Rgba8::RED;
	WARNING = Rgba8::YELLOW;
	INFO_MAJOR = Rgba8::BLUE;
	INFO_MINOR = Rgba8::GREEN;
	COMMAND = Rgba8::WHITE;

	if (NetSystem::Exists())
	{
		RemoteConsoleConfig config;
		config.m_console = this;
		m_remoteConsole = new RemoteConsole(config);
		m_remoteConsole->Startup();
	}

	m_textFont = m_config.m_renderer->CreateOrGetBitmapFont(m_config.m_fontFilePath.c_str());
	//SubscribeEventCallbackFunction("Test", TestCommand);
	SubscribeEventCallbackFunction("KeyPressed", Event_KeyPressed);
	SubscribeEventCallbackFunction("CharInput", Event_CharInput);
	SubscribeEventCallbackFunction("clear", Command_Clear);
	SubscribeEventCallbackFunction("help", Command_Help);
	SubscribeEventCallbackFunction("debugrenderclear", Command_DebugRenderClear);
	SubscribeEventCallbackFunction("debugrendertoggle", Command_DebugRenderToggle);

	if (m_remoteConsole)
	{
		SubscribeEventCallbackFunction("rc_join", Command_JoinHost);
		SubscribeEventCallbackFunction("rc_host", Command_HostAtPort);
		SubscribeEventCallbackFunction("rc_leave", Command_Leave);
		SubscribeEventCallbackFunction("rc_echo", Command_Echo);
		SubscribeEventCallbackFunction("rc", Command_RCCommand);
	}

	m_caretStopwatch.Start(&m_clock, caretBlinkTime);
	m_commandHistory.resize(m_config.m_maxCommandHistory);

	AddLine(COMMAND, "Type help for a list of commands.");
	m_uiScreenSize = IntVec2(g_gameConfigBlackboard.GetValue("screenSizeWidth", m_uiScreenSize.x), g_gameConfigBlackboard.GetValue("screenSizeHeight", m_uiScreenSize.x));
}

void DevConsole::Shutdown()
{
	if (m_remoteConsole)
	{
		m_remoteConsole->Shutdown();
		delete m_remoteConsole;
		m_remoteConsole = nullptr;
	}
}

void DevConsole::BeginFrame()
{
	m_frameNumber++;
	if (m_caretStopwatch.CheckDurationElapsedAndDecrement())
	{
		m_canRenderCaret = !m_canRenderCaret;
	}

	HandleDebugInput();
	//ShowClockInfo();

	if (m_remoteConsole)
	{
		m_remoteConsole->Update();
	}
}

void DevConsole::EndFrame()
{

}

void DevConsole::Execute(const std::string& consoleCommandText)
{
	EventArgs eventArgs;
	Strings commandAndArgs = SplitStringOnDelimiter(consoleCommandText, ' ');
	std::string commandArg = commandAndArgs[0];
	if (commandAndArgs.size() > 1)
	{
		for (int i = 1; i < commandAndArgs.size(); i++)
		{
			Strings argsAndValue = SplitStringOnDelimiter(commandAndArgs[i], '=');
			if (argsAndValue.size() > 1)
			{
				std::string args = argsAndValue[0];
				std::string argsValue = argsAndValue[1].substr(1, argsAndValue[1].size() - 2);
				eventArgs.SetValue(args, argsValue);
			}
		}
	}

	FireEvent(commandArg, eventArgs);
	g_theConsole->AddLine(COMMAND, consoleCommandText);
}

void DevConsole::AddLine(const Rgba8& color, const std::string& text)
{
	DevConsoleLine line;
	line.m_text = text;
	line.m_textColor = color;
	line.m_frameNumber = m_frameNumber;

	m_devConsoleLineMutex.lock();
	m_lines.push_back(line);
	m_devConsoleLineMutex.unlock();
}

void DevConsole::Render(const AABB2& bounds, Renderer* rendererOverride) const
{
	switch (m_mode)
	{
	case DevConsoleMode::OPEN_FULL:
		Renderer* rendererToUse = m_config.m_renderer;
		if (rendererOverride)
			rendererToUse = rendererOverride;
		Render_OpenFull(bounds, *rendererToUse, *m_textFont, m_config.m_fontAspect);

		if (m_remoteConsole)
		{
			m_remoteConsole->Render(*rendererToUse, bounds, *m_textFont);
		}
		break;
	}
}

void DevConsole::RemoveAllLines()
{
	m_lines.clear();
}

void DevConsole::AddCharacterToInputStream(int charCode)
{
	if ((charCode >= 32) && (charCode <= 126) && (charCode != '`') && (charCode != '~'))
	{
		auto itr = m_inputText.begin() + m_caretPosition;
		m_inputText.insert(itr, static_cast<char>(charCode));
		IncrementCaretPosition();
		ResetCaret();
	}
}

void DevConsole::IncrementCaretPosition()
{
	m_caretPosition++;
}

void DevConsole::HandleKeyPresses(unsigned char keyCode)
{
	if (keyCode == KEYCODE_ESCAPE)
	{
		m_inputText.clear();
		m_caretPosition = 0;
	}
	else if (keyCode == KEYCODE_ENTER)
	{
		Execute(m_inputText);
		m_commandHistoryInsertionIndex = m_commandHistoryInsertionIndex % m_config.m_maxCommandHistory;
		m_commandHistory[m_commandHistoryInsertionIndex] = m_inputText;
		m_inputText.clear();
		m_caretPosition = 0;
		m_commandHistoryInsertionIndex++;
		m_commandHistoryScrollIndex = m_commandHistoryInsertionIndex;
	}
	else if (keyCode == KEYCODE_LEFTARROW)
	{
		m_caretPosition--;
		if (m_caretPosition <= 0)
			m_caretPosition = 0;
		ResetCaret();
	}
	else if (keyCode == KEYCODE_RIGHTARROW)
	{
		m_caretPosition++;
		if (m_caretPosition >= m_inputText.size())
			m_caretPosition = static_cast<int>(m_inputText.size());
		ResetCaret();
	}
	else if (keyCode == KEYCODE_UPARROW)
	{
		m_commandHistoryScrollIndex--;
		if (m_commandHistoryScrollIndex <= 0)
			m_commandHistoryScrollIndex = 0;

		m_inputText = m_commandHistory[m_commandHistoryScrollIndex];
		m_caretPosition = static_cast<int>(m_inputText.size());
		ResetCaret();
	}
	else if (keyCode == KEYCODE_DOWNARROW)
	{
		m_commandHistoryScrollIndex++;
		if (m_commandHistoryScrollIndex > m_commandHistoryInsertionIndex)
			m_commandHistoryScrollIndex = m_commandHistoryInsertionIndex;

		m_inputText = m_commandHistory[m_commandHistoryScrollIndex];
		m_caretPosition = static_cast<int>(m_inputText.size());
		ResetCaret();
	}
	else if (keyCode == KEYCODE_HOME)
	{
		m_caretPosition = 0;
		ResetCaret();
	}
	else if (keyCode == KEYCODE_END)
	{
		m_caretPosition = static_cast<int>(m_inputText.size());
		ResetCaret();
	}
	else if (keyCode == KEYCODE_DELETE)
	{
		int indexOfCharToDelete = m_caretPosition;
		if (indexOfCharToDelete >= m_inputText.size())
			return;

		m_inputText.erase(m_inputText.begin() + indexOfCharToDelete);
		m_caretPosition = indexOfCharToDelete;
		ResetCaret();
	}
	else if (keyCode == KEYCODE_BACKSPACE)
	{
		int indexOfCharToDelete = m_caretPosition - 1;
		if (indexOfCharToDelete < 0)
			return;

		m_inputText.erase(m_inputText.begin() + indexOfCharToDelete);
		m_caretPosition = indexOfCharToDelete;
		ResetCaret();
	}
}

void DevConsole::ResetCaret()
{
	m_canRenderCaret = true;
	m_caretStopwatch.Restart();
}

void DevConsole::HandleDebugInput()
{
	if (g_theInput->WasKeyJustPressed(KEYCODE_F1))
		m_clock.SetTimeDilation(1.0);
	if (g_theInput->WasKeyJustPressed(KEYCODE_F2))
		m_clock.SetTimeDilation(2.0);
	if (g_theInput->WasKeyJustPressed(KEYCODE_F3))
		m_clock.SetTimeDilation(0.5);
}

DevConsoleMode DevConsole::GetMode() const
{
	return m_mode;
}

void DevConsole::SetMode(DevConsoleMode mode)
{
	m_mode = mode;
	switch (mode)
	{
	case DevConsoleMode::HIDDEN:
		break;
	case DevConsoleMode::OPEN_FULL:
		m_caretStopwatch.Restart();
		break;
	default:
		break;
	}
}

void DevConsole::ToggleMode(DevConsoleMode mode)
{
	if (m_mode == DevConsoleMode::HIDDEN)
		SetMode(mode);
	else
		SetMode(DevConsoleMode::HIDDEN);
}

RemoteConsole* DevConsole::GetRemoteConsole() const
{
	return m_remoteConsole;
}

void DevConsole::Render_OpenFull(const AABB2& bounds, Renderer& renderer, BitmapFont& font, float fontAspect) const
{
	renderer.SetBlendMode(BlendMode::ALPHA);

	int maxLines = m_config.m_maxLinesToPrint < m_lines.size() ? m_config.m_maxLinesToPrint : (int)m_lines.size();
	float textCellHeight = m_config.m_fontCellHeight;
	if (bounds.m_maxs.y < (maxLines * m_config.m_fontCellHeight))
		textCellHeight *= bounds.m_maxs.y / (maxLines * m_config.m_fontCellHeight);

	std::vector<Vertex_PCU> verts;
	//add verts for dev console BG
	AddVertsForAABB2D(verts, bounds, Rgba8(0, 0, 0, 127));

	//add verts for dev console's input box
	AABB2 inputBox(bounds.m_mins, Vec2(bounds.m_maxs.x, textCellHeight));
	AddVertsForAABB2D(verts, inputBox, Rgba8(0, 0, 0, 200));
	
	//add verts for caret line
	if (m_canRenderCaret)
	{
		std::string subStringTillCaretCurrentPosition = m_inputText.substr(0, m_caretPosition);
		float caretXPosition = font.GetTextWidth(textCellHeight, subStringTillCaretCurrentPosition, fontAspect);
		LineSegment2 caretLine(Vec2(caretXPosition, inputBox.m_mins.y), Vec2(caretXPosition, inputBox.m_maxs.y));
		AddVertsForLineSegment2D(verts, caretLine, caretLineThickness, Rgba8::WHITE);
	}

	renderer.BindTexture(nullptr);
	renderer.DrawVertexArray((int)verts.size(), verts.data());
	
	verts.clear();
	int startLine = Clamp(int(m_lines.size() - maxLines), 0, int(m_lines.size()));
	for (int i = 0; i < maxLines; i++)
	{
		AABB2 textBox;
		textBox.m_mins = Vec2(bounds.m_mins.x, (maxLines - i) * textCellHeight);
		textBox.m_maxs = Vec2(bounds.m_maxs.x, textBox.m_mins.y + textCellHeight);
		std::string textToPrint = m_lines[startLine + i].m_text;
		/*if (m_lines[i].m_textColor != COMMAND)
		{
			std::string frameNumber = Stringf(" #%d", m_lines[startLine + i].m_frameNumber);
			textToPrint.append(frameNumber);
		}*/
		font.AddVertsForTextInBox2D(verts, textBox, textCellHeight, textToPrint, m_lines[i].m_textColor,
			fontAspect, Vec2::ZERO);
	}
	font.AddVertsForTextInBox2D(verts, inputBox, textCellHeight, m_inputText, Rgba8::WHITE, fontAspect, Vec2::ZERO);
	renderer.BindTexture(&font.GetTexture());
	renderer.DrawVertexArray((int)verts.size(), verts.data());
}

//bool DevConsole::TestCommand(EventArgs& args)
//{
//	UNUSED(args);
//	g_theConsole->AddLine(INFO_MAJOR, "Test command");
//	return false;
//}

bool DevConsole::Event_KeyPressed(EventArgs& args)
{
	if (g_theConsole->GetMode() == DevConsoleMode::HIDDEN)
		return false;

	unsigned char keyCode = static_cast<unsigned char>(args.GetValue("keyCode", 0));
	g_theConsole->HandleKeyPresses(keyCode);
	return false;
}

bool DevConsole::Event_CharInput(EventArgs& args)
{
	if (g_theConsole->GetMode() == DevConsoleMode::HIDDEN)
		return false;

	int charCode = args.GetValue("charCode", 0);
	g_theConsole->AddCharacterToInputStream(charCode);
	//g_theConsole->IncrementCaretPosition();
	return false;
}

bool DevConsole::Command_Clear(EventArgs& args)
{
	UNUSED(args);
	g_theConsole->RemoveAllLines();
	return false;
}

bool DevConsole::Command_Help(EventArgs& args)
{
	g_theConsole->AddLine(g_theConsole->COMMAND, "---- Registered Commands ----");
	std::vector<std::string> registeredCommands;
	g_theEventSystem->GetRegisteredEventNames(registeredCommands);

	std::string filterArgValue = args.GetValue("Filter", "");
	if (filterArgValue != "")
	{
		g_theConsole->AddLine(g_theConsole->COMMAND, Stringf("---- Filtering output to \"%s\" ----", filterArgValue.c_str()));
	}

	for (int i = 0; i < registeredCommands.size(); i++)
	{
		std::size_t found = registeredCommands[i].find(filterArgValue);
		if (found != std::string::npos)
		{
			g_theConsole->AddLine(g_theConsole->COMMAND, registeredCommands[i]);
		}
	}
	return false;
}

bool DevConsole::Command_DebugRenderClear(EventArgs& args)
{
	UNUSED(args);
	DebugRenderClear();
	return false;
}

bool DevConsole::Command_DebugRenderToggle(EventArgs& args)
{
	UNUSED(args);
	DebugRenderToggleVisibility();
	return false;
}

bool DevConsole::Command_JoinHost(EventArgs& args)
{
	std::string hostAddressString = args.GetValue("addr", "");
	NetAddress hostAddress = NetAddress::FromString(hostAddressString);
	g_theConsole->AddLine(g_theConsole->INFO_MINOR, Stringf("Trying to connect to %s", hostAddressString.c_str()));
	g_theConsole->GetRemoteConsole()->ConnectToHost(hostAddress);
	return true;
}

bool DevConsole::Command_HostAtPort(EventArgs& args)
{
	std::string hostPort = args.GetValue("port", "");
	int port = atoi(hostPort.c_str());
	g_theConsole->GetRemoteConsole()->AttemptToHost(static_cast<uint16_t>(port), false);
	return true;
}

bool DevConsole::Command_Leave(EventArgs& args)
{
	UNUSED(args);
	g_theConsole->GetRemoteConsole()->StopHosting();
	g_theConsole->GetRemoteConsole()->CloseAllConnections();
	return false;
}

bool DevConsole::Command_Echo(EventArgs& args)
{
	int connIndex = atoi(args.GetValue("idx", "0").c_str());
	std::string echoMessage = args.GetValue("msg", "");
	g_theConsole->GetRemoteConsole()->SendCommand(connIndex, echoMessage, true);
	return true;
}

bool DevConsole::Command_RCCommand(EventArgs& args)
{
	int connIndex = atoi(args.GetValue("idx", "0").c_str());
	std::string commandString = args.GetValue("cmd", "");
	g_theConsole->GetRemoteConsole()->SendCommand(connIndex, commandString, false);
	return true;
}

void DevConsole::ShowClockInfo()
{
	std::string clockInfo = Stringf("DevConsole Clock | Time: %.2f FPS: %.2f Scale: %.2f", m_clock.GetTotalTime(), 1.f / m_clock.GetDeltaTime(), m_clock.GetTimeDilation());
	DebugAddScreenText(clockInfo, Vec2(m_uiScreenSize.x - 550.f, m_uiScreenSize.y - 22.f), FLT_MIN, Vec2::ZERO, 10.f, Rgba8::WHITE, Rgba8::WHITE);
}
