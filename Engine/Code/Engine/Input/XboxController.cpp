#include "Engine/Input/XboxController.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <windows.h>
#include <xinput.h>
#pragma comment (lib, "xinput9_1_0")

XboxController::XboxController()
{
	m_leftJoystick.SetDeadZoneThresholds(0.3f, 0.95f);
	m_rightJoystick.SetDeadZoneThresholds(0.3f, 0.95f);
}

bool XboxController::IsConnected() const
{
	return m_isConnected;
}

int XboxController::GetControllerID() const
{
	return m_id;
}

float XboxController::GetLeftTrigger() const
{
	return m_leftTrigger;
}

float XboxController::GetRightTrigger() const
{
	return m_rightTrigger;
}

const AnalogJoystick& XboxController::GetLeftJoystick() const
{
	return m_leftJoystick;
}

const AnalogJoystick& XboxController::GetRightJoystick() const
{
	return m_rightJoystick;
}

bool XboxController::IsButtonDown(XboxButtonID button) const
{
	return m_buttons[button].m_currentState;
}

bool XboxController::WasButtonJustPressed(XboxButtonID button) const
{
	return (m_buttons[button].m_currentState && !m_buttons[button].m_previousFrameState);
}

bool XboxController::WasButtonJustReleased(XboxButtonID button) const
{
	return (!m_buttons[button].m_currentState && m_buttons[button].m_previousFrameState);
}

void XboxController::Update()
{
	XINPUT_STATE xboxControllerState = {};
	DWORD errorState = XInputGetState(m_id, &xboxControllerState);
	if (errorState == ERROR_SUCCESS)
	{
		m_isConnected = true;

		XINPUT_GAMEPAD state = xboxControllerState.Gamepad;
		//DebuggerPrintf("buttons state = %5i \n", state.wButtons);
		UpdateTriggers(m_leftTrigger, state.bLeftTrigger);
		UpdateTriggers(m_rightTrigger, state.bRightTrigger);
		//DebuggerPrintf("left trigger = %f, right trigger = %f \n", m_leftTrigger, m_rightTrigger);

		UpdateButton(XBOX_BUTTON_A, state.wButtons, XINPUT_GAMEPAD_A);
		UpdateButton(XBOX_BUTTON_B, state.wButtons, XINPUT_GAMEPAD_B);
		UpdateButton(XBOX_BUTTON_X, state.wButtons, XINPUT_GAMEPAD_X);
		UpdateButton(XBOX_BUTTON_Y, state.wButtons, XINPUT_GAMEPAD_Y);
		UpdateButton(XBOX_BUTTON_LEFT_SHOULDER, state.wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER);
		UpdateButton(XBOX_BUTTON_RIGHT_SHOULDER, state.wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		UpdateButton(XBOX_BUTTON_DPAD_UP, state.wButtons, XINPUT_GAMEPAD_DPAD_UP);
		UpdateButton(XBOX_BUTTON_DPAD_DOWN, state.wButtons, XINPUT_GAMEPAD_DPAD_DOWN);
		UpdateButton(XBOX_BUTTON_DPAD_LEFT, state.wButtons, XINPUT_GAMEPAD_DPAD_LEFT);
		UpdateButton(XBOX_BUTTON_DPAD_RIGHT, state.wButtons, XINPUT_GAMEPAD_DPAD_RIGHT);
		UpdateButton(XBOX_BUTTON_L3, state.wButtons, XINPUT_GAMEPAD_LEFT_THUMB);
		UpdateButton(XBOX_BUTTON_R3, state.wButtons, XINPUT_GAMEPAD_RIGHT_THUMB);
		UpdateButton(XBOX_BUTTON_START, state.wButtons, XINPUT_GAMEPAD_START);
		UpdateButton(XBOX_BUTTON_BACK, state.wButtons, XINPUT_GAMEPAD_BACK);

		UpdateJoystick(m_leftJoystick, state.sThumbLX, state.sThumbLY);
		UpdateJoystick(m_rightJoystick, state.sThumbRX, state.sThumbRY);

		//DebuggerPrintf("left stick corrected = %f, %f \n", m_leftJoystick.GetPosition().x, m_leftJoystick.GetPosition().y);
		//DebuggerPrintf("left stick = %f, %f \n", m_leftJoystick.GetRawUncorrectedPosition().x, m_leftJoystick.GetRawUncorrectedPosition().y);
	}
	else
	{
		m_isConnected = false;
		Reset();
	}
}

void XboxController::Reset()
{
	m_leftTrigger = 0.f;
	m_rightTrigger = 0.f;

	for (int i = 0; i < (int)NUM_XBOX_BUTTONS; i++)
	{
		m_buttons[(XboxButtonID)i].m_currentState = false;
	}

	m_leftJoystick.Reset();
	m_rightJoystick.Reset();

}

void XboxController::UpdateTriggers(float& out_triggerValue, unsigned char rawValue)
{
	out_triggerValue = RangeMap(rawValue, 0.f, 255.f, 0.f, 1.f);
}

void XboxController::UpdateButton(XboxButtonID buttonID, unsigned short buttonFlags, unsigned short buttonMask)
{
	m_buttons[buttonID].m_currentState = ((buttonFlags & buttonMask) == buttonMask);
}

void XboxController::UpdateJoystick(AnalogJoystick& out_analogStick, short rawX, short rawY)
{
	out_analogStick.UpdatePositons(rawX, rawY);
}
