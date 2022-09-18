#pragma once
#include "Engine/Input/KeyButtonState.hpp"
#include "Engine/Input/AnalogJoystick.hpp"

enum XboxButtonID
{
	XBOX_BUTTON_A = 0,
	XBOX_BUTTON_B,
	XBOX_BUTTON_X,
	XBOX_BUTTON_Y,
	XBOX_BUTTON_LEFT_SHOULDER,
	XBOX_BUTTON_RIGHT_SHOULDER,
	XBOX_BUTTON_DPAD_UP,
	XBOX_BUTTON_DPAD_DOWN,
	XBOX_BUTTON_DPAD_LEFT,
	XBOX_BUTTON_DPAD_RIGHT,
	XBOX_BUTTON_START,
	XBOX_BUTTON_BACK,
	XBOX_BUTTON_L3,
	XBOX_BUTTON_R3,

	NUM_XBOX_BUTTONS
};

class XboxController
{

	friend class InputSystem;

public:
	XboxController();
	bool IsConnected() const;
	int GetControllerID() const;
	float GetLeftTrigger() const;
	float GetRightTrigger() const;
	const AnalogJoystick& GetLeftJoystick() const;
	const AnalogJoystick& GetRightJoystick() const;
	bool IsButtonDown(XboxButtonID button) const;
	bool WasButtonJustPressed(XboxButtonID button) const;
	bool WasButtonJustReleased(XboxButtonID button) const;

private:
	int m_id = -1;
	bool m_isConnected = false;
	float m_leftTrigger = 0.f;
	float m_rightTrigger = 0.f;
	KeyButtonState m_buttons[(int)XboxButtonID::NUM_XBOX_BUTTONS];
	AnalogJoystick m_leftJoystick;
	AnalogJoystick m_rightJoystick;

private:
	void Update();
	void Reset();
	void UpdateTriggers(float& out_triggerValue, unsigned char rawValue);
	void UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonMask);
	void UpdateJoystick(AnalogJoystick& out_analogStick, short rawX, short rawY);
};