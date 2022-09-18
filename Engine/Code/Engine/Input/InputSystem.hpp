#pragma once
#include "Engine/Input/KeyButtonState.hpp"
#include "Engine/Input/XboxController.hpp"

extern const unsigned char KEYCODE_F1;
extern const unsigned char KEYCODE_F2;
extern const unsigned char KEYCODE_F3;
extern const unsigned char KEYCODE_F4;
extern const unsigned char KEYCODE_F5;
extern const unsigned char KEYCODE_F6;
extern const unsigned char KEYCODE_F7;
extern const unsigned char KEYCODE_F8;
extern const unsigned char KEYCODE_F9;
extern const unsigned char KEYCODE_F10;
extern const unsigned char KEYCODE_F11;
extern const unsigned char KEYCODE_F12;
extern const unsigned char KEYCODE_ESCAPE;
extern const unsigned char KEYCODE_UPARROW;
extern const unsigned char KEYCODE_DOWNARROW;
extern const unsigned char KEYCODE_LEFTARROW;
extern const unsigned char KEYCODE_RIGHTARROW;
extern const unsigned char KEYCODE_LEFT_MOUSE;
extern const unsigned char KEYCODE_RIGHT_MOUSE;
extern const unsigned char KEYCODE_TILDE;
extern const unsigned char KEYCODE_SPACE;
extern const unsigned char KEYCODE_ENTER;
extern const unsigned char KEYCODE_HOME;
extern const unsigned char KEYCODE_END;
extern const unsigned char KEYCODE_DELETE;
extern const unsigned char KEYCODE_BACKSPACE;
extern const unsigned char KEYCODE_SHIFT;

struct InputSystemConfig
{

};

class InputSystem
{
public:
	InputSystem(const InputSystemConfig& config);
	void Startup();
	void Shutdown();
	void BeginFrame();
	void EndFrame();
	void HandleKeyPressed(unsigned char keyCode);
	void HandleKeyReleased(unsigned char keyCode);
	void HandleCharInput(int charCode);
	bool IsKeyDown(unsigned char keyCode);
	bool WasKeyJustPressed(unsigned char keyCode);
	bool WasKeyJustReleased(unsigned char keyCode);
	void ConsumeKeyJustPressed(unsigned char keyCode);
	void ClearKeyInput();
	const XboxController& GetController(int controllerID);
	void SetMouseMode(bool hidden, bool clipped, bool relative);
	Vec2 GetMouseClientPosition() const;
	Vec2 GetMouseClientDelta() const;
	void SetMouseWheelDelta(short delta);
	float GetMouseWheelDelta() const;

protected:
	KeyButtonState m_keyStates[256];
	XboxController m_controllers[4];
	Vec2 m_currentMousePosition;
	Vec2 m_previousMousePosition;
	bool m_mouseCursorCurrentlyHidden = false;
	float m_mouseWheelDelta = 0.f;

private:
	InputSystemConfig m_config;
};