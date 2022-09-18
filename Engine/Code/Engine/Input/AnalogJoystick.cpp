#include "Engine/Input/AnalogJoystick.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

Vec2 AnalogJoystick::GetPosition() const
{
	return m_correctedPosition;
}

float AnalogJoystick::GetMagnitude() const
{
	return m_correctedPosition.GetLength();
}

float AnalogJoystick::GetOrientationDegrees() const
{
	return m_correctedPosition.GetOrientationDegrees();
}

Vec2 AnalogJoystick::GetRawUncorrectedPosition() const
{
	return m_rawPosition;
}

float AnalogJoystick::GetInnerDeadZoneFraction() const
{
	return m_innerDeadZoneFraction;
}

float AnalogJoystick::GetOuterDeadZoneFraction() const
{
	return m_outerDeadZoneFraction;
}

void AnalogJoystick::Reset()
{
	m_rawPosition = Vec2(0.f, 0.f);
	m_correctedPosition = Vec2(0.f, 0.f);
}

void AnalogJoystick::SetDeadZoneThresholds(float normalizedInnerDeadZoneFraction, float normalizedOuterDeadZoneFraction)
{
	m_innerDeadZoneFraction = normalizedInnerDeadZoneFraction;
	m_outerDeadZoneFraction = normalizedOuterDeadZoneFraction;
}

void AnalogJoystick::UpdatePositons(float rawX, float rawY)
{
	//range map raw values -32768, 32768 to normalized -1, 1
	float rawNormalizedX = RangeMap(rawX, -32768.f, 32767.f, -1.f, 1.f);
	float rawNormalizedY = RangeMap(rawY, -32768.f, 32767.f, -1.f, 1.f);
	m_rawPosition = Vec2(rawNormalizedX, rawNormalizedY);

	//convert to polar
	float magnitude = m_rawPosition.GetLength();
	float orientation = m_rawPosition.GetOrientationDegrees();

	//range map clamp the magnitude
	float clampedLength = RangeMapClamped(magnitude, m_innerDeadZoneFraction, m_outerDeadZoneFraction, 0.f, 1.f);

	//convert to Cartesian from polar
	m_correctedPosition = Vec2::MakeFromPolarDegrees(orientation, clampedLength);

	

	//DebuggerPrintf("corrected = %f, %f \n", m_correctedPosition.x, m_correctedPosition.y);
}
