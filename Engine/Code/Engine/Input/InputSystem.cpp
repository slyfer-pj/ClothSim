#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Renderer/Window.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <windows.h>

const unsigned char KEYCODE_F1 = VK_F1;
const unsigned char KEYCODE_F2 = VK_F2;
const unsigned char KEYCODE_F3 = VK_F3;
const unsigned char KEYCODE_F4 = VK_F4;
const unsigned char KEYCODE_F5 = VK_F5;
const unsigned char KEYCODE_F6 = VK_F6;
const unsigned char KEYCODE_F7 = VK_F7;
const unsigned char KEYCODE_F8 = VK_F8;
const unsigned char KEYCODE_F9 = VK_F9;
const unsigned char KEYCODE_F10 = VK_F10;
const unsigned char KEYCODE_F11 = VK_F11;
const unsigned char KEYCODE_F12 = VK_F12;
const unsigned char KEYCODE_ESCAPE = VK_ESCAPE;
const unsigned char KEYCODE_UPARROW = VK_UP;
const unsigned char KEYCODE_DOWNARROW = VK_DOWN;
const unsigned char KEYCODE_LEFTARROW = VK_LEFT;
const unsigned char KEYCODE_RIGHTARROW = VK_RIGHT;
const unsigned char KEYCODE_LEFT_MOUSE = VK_LBUTTON;
const unsigned char KEYCODE_RIGHT_MOUSE = VK_RBUTTON;
const unsigned char KEYCODE_TILDE = VK_OEM_3;
const unsigned char KEYCODE_SPACE = VK_SPACE;
const unsigned char KEYCODE_ENTER = VK_RETURN;
const unsigned char KEYCODE_HOME = VK_HOME;
const unsigned char KEYCODE_END = VK_END;
const unsigned char KEYCODE_DELETE = VK_DELETE;
const unsigned char KEYCODE_BACKSPACE = VK_BACK;
const unsigned char KEYCODE_SHIFT = VK_SHIFT;


InputSystem::InputSystem(const InputSystemConfig& config)
	:m_config(config)
{

}

void InputSystem::Startup()
{
	SetMouseMode(false, false, false);
}

void InputSystem::Shutdown()
{

}

void InputSystem::BeginFrame()
{
	for (int controllerNum = 0; controllerNum < 4; controllerNum++)
	{
		m_controllers[controllerNum].m_id = controllerNum;
		m_controllers[controllerNum].Update();
	}
}

void InputSystem::EndFrame()
{
	for (int index = 0; index < 256; index++)
		m_keyStates[index].m_previousFrameState = m_keyStates[index].m_currentState;

	for (int controllerNum = 0; controllerNum < 4; controllerNum++)
	{
		for (int buttons = 0; buttons < NUM_XBOX_BUTTONS; buttons++)
			m_controllers[controllerNum].m_buttons[buttons].m_previousFrameState = m_controllers[controllerNum].m_buttons[buttons].m_currentState;
	}

	SetMouseWheelDelta(0);
}

void InputSystem::HandleKeyPressed(unsigned char keyCode)
{
	m_keyStates[keyCode].m_currentState = true;
	EventArgs args;
	args.SetValue("keyCode", Stringf("%d", keyCode));
	FireEvent("KeyPressed", args);
}

void InputSystem::HandleKeyReleased(unsigned char keyCode)
{
	m_keyStates[keyCode].m_currentState = false;
}

void InputSystem::HandleCharInput(int charCode)
{
	EventArgs args;
	args.SetValue("charCode", Stringf("%d", charCode));
	FireEvent("CharInput", args);
}

bool InputSystem::IsKeyDown(unsigned char keyCode)
{
	return m_keyStates[keyCode].m_currentState;
}

bool InputSystem::WasKeyJustPressed(unsigned char keyCode)
{
	return (m_keyStates[keyCode].m_currentState && !m_keyStates[keyCode].m_previousFrameState);
}

bool InputSystem::WasKeyJustReleased(unsigned char keyCode)
{
	return (!m_keyStates[keyCode].m_currentState && m_keyStates[keyCode].m_previousFrameState);
}

void InputSystem::ConsumeKeyJustPressed(unsigned char keyCode)
{
	m_keyStates[keyCode].m_currentState = false;
}

void InputSystem::ClearKeyInput()
{
	for (int index = 0; index < 256; index++)
		m_keyStates[index].m_currentState = false;

	for (int controllerNum = 0; controllerNum < 4; controllerNum++)
	{
		for (int buttons = 0; buttons < NUM_XBOX_BUTTONS; buttons++)
			m_controllers[controllerNum].m_buttons[buttons].m_currentState = false;
	}
}

const XboxController& InputSystem::GetController(int controllerID)
{
	return m_controllers[controllerID];
}

void InputSystem::SetMouseMode(bool hidden, bool clipped, bool relative)
{
	CURSORINFO pci = { 0 };
	pci.cbSize = sizeof(CURSORINFO);
	GetCursorInfo(&pci);

	Window* mainWindow = Window::GetWindowContext();
	IntVec2 clientDimensions = mainWindow->GetClientDimensions();

	if ((pci.flags == 0 && !hidden) || (pci.flags != 0 && hidden))
	{
		if (hidden)
		{
			int cursorHiddenValue = ShowCursor(FALSE);
			while (cursorHiddenValue >= 0)
				cursorHiddenValue = ShowCursor(FALSE);
			m_mouseCursorCurrentlyHidden = true;

			POINT pt = { 0 };
			pt.x = int(clientDimensions.x * 0.5f);
			pt.y = int(clientDimensions.y * 0.5f);
			ClientToScreen((HWND)mainWindow->GetOSWindowHandle(), &pt);
			SetCursorPos(pt.x, pt.y);
		}
		else
		{
			int cursorHiddenValue = ShowCursor(TRUE);
			while (cursorHiddenValue < 0)
				cursorHiddenValue = ShowCursor(TRUE);
			m_mouseCursorCurrentlyHidden = false;
		}
	}

	if (clipped || relative)
	{
		RECT windowRect = { 0 };
		windowRect.left = 0;
		windowRect.right = clientDimensions.x;
		windowRect.bottom = 0;
		windowRect.top = clientDimensions.y;
		ClipCursor(&windowRect);
	}

	if (relative)
	{
		POINT prevPos;
		GetCursorPos(&prevPos);
		ScreenToClient((HWND)mainWindow->GetOSWindowHandle(), &prevPos);
		m_previousMousePosition = Vec2((float)prevPos.x, (float)prevPos.y);

		POINT pt = { 0 };
		pt.x = int(clientDimensions.x * 0.5f);
		pt.y = int(clientDimensions.y * 0.5f);
		ClientToScreen((HWND)mainWindow->GetOSWindowHandle(), &pt);
		SetCursorPos(pt.x, pt.y);

		POINT currPos;
		GetCursorPos(&currPos);
		ScreenToClient((HWND)mainWindow->GetOSWindowHandle(), &currPos);
		m_currentMousePosition = Vec2((float)currPos.x, (float)currPos.y);
	}
	
}

//void InputSystem::RecordMousePosition()
//{
//	Window* mainWindow = Window::GetWindowContext();
//	POINT prevPos;
//	GetCursorPos(&prevPos);
//	ScreenToClient((HWND)mainWindow->GetOSWindowHandle(), &prevPos);
//	m_previousMousePosition = Vec2((float)prevPos.x, (float)prevPos.y);
//	//m_previousMousePosition = m_currentMousePosition;
//	POINT pt = { 0 };
//	GetCursorPos(&pt);
//	//DebuggerPrintf("pt = %d %d \n", pt.x, pt.y);
//	ScreenToClient((HWND)mainWindow->GetOSWindowHandle(), &pt);
//	m_currentMousePosition = Vec2(float(pt.x), float(pt.y));
//	DebuggerPrintf("Current = %f, %f   \n", m_currentMousePosition.x, m_currentMousePosition.y /*m_previousMousePosition.x, m_previousMousePosition.y*/);
//}

Vec2 InputSystem::GetMouseClientPosition() const
{
	return m_currentMousePosition;
}

Vec2 InputSystem::GetMouseClientDelta() const
{
	if (!m_mouseCursorCurrentlyHidden)
		return Vec2::ZERO;

	return (m_currentMousePosition - m_previousMousePosition);
}

void InputSystem::SetMouseWheelDelta(short delta)
{
	m_mouseWheelDelta = (float)delta;
}

float InputSystem::GetMouseWheelDelta() const
{
	return m_mouseWheelDelta;
}

