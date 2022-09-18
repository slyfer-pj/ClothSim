#pragma once
#include <string>
#include <vector>
#include <mutex>
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Math/IntVec2.hpp"

struct AABB2;
class Renderer;
class BitmapFont;
class NamedStrings;
class RemoteConsole;
typedef NamedStrings EventArgs;

struct DevConsoleLine
{
	std::string m_text;
	Rgba8 m_textColor = Rgba8::WHITE;
	int m_frameNumber = 0;
};

enum class DevConsoleMode
{
	HIDDEN,
	OPEN_FULL
};

struct DevConsoleConfig
{
	Renderer* m_renderer = nullptr;
	std::string m_fontFilePath;
	float m_fontAspect = 0.f;
	float m_fontCellHeight = 0.f;
	int m_maxLinesToPrint = 10;
	int m_maxCommandHistory = 128;
};

class DevConsole
{
public:
	DevConsole(const DevConsoleConfig& config);
	~DevConsole();
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();

	void Execute(const std::string& consoleCommandText);
	void AddLine(const Rgba8& color, const std::string& text);
	void Render(const AABB2& bounds, Renderer* rendererOverride = nullptr) const;

	DevConsoleMode GetMode() const;
	void SetMode(DevConsoleMode mode);
	void ToggleMode(DevConsoleMode mode);
	RemoteConsole* GetRemoteConsole() const;

	Rgba8 ERRORTEXT;
	Rgba8 WARNING;
	Rgba8 INFO_MAJOR;
	Rgba8 INFO_MINOR;
	Rgba8 COMMAND;

protected:
	void Render_OpenFull(const AABB2& bounds, Renderer& renderer, BitmapFont& font, float fontAspect = 1.f) const;
	void RemoveAllLines();
	void AddCharacterToInputStream(int charCode);
	void IncrementCaretPosition();
	void HandleKeyPresses(unsigned char keyCode);
	void ResetCaret();
	void HandleDebugInput();
	void ShowClockInfo();

	//static bool TestCommand(EventArgs& args);
	static bool Event_KeyPressed(EventArgs& args);
	static bool Event_CharInput(EventArgs& args);
	static bool Command_Clear(EventArgs& args);
	static bool Command_Help(EventArgs& args);
	static bool Command_DebugRenderClear(EventArgs& args);
	static bool Command_DebugRenderToggle(EventArgs& args);

	//remote console commands
	static bool Command_JoinHost(EventArgs& args);
	static bool Command_HostAtPort(EventArgs& args);
	static bool Command_Leave(EventArgs& args);
	static bool Command_Echo(EventArgs& args);
	static bool Command_RCCommand(EventArgs& args);

protected:
	DevConsoleConfig m_config;
	DevConsoleMode m_mode = DevConsoleMode::HIDDEN;
	std::vector<DevConsoleLine> m_lines;
	std::mutex m_devConsoleLineMutex;
	int m_frameNumber = 0;
	BitmapFont* m_textFont = nullptr;
	Clock m_clock;
	std::string m_inputText;
	int m_caretPosition = 0;
	bool m_caretVisible = true;
	Stopwatch m_caretStopwatch;
	bool m_canRenderCaret = false;
	std::vector<std::string> m_commandHistory;
	int m_commandHistoryInsertionIndex = 0;
	int m_commandHistoryScrollIndex = -1;
	IntVec2 m_uiScreenSize = IntVec2::ZERO;
	RemoteConsole* m_remoteConsole = nullptr;
};