#pragma once
//#include "Engine/Math/Mat44.hpp"

struct Mat44;
struct Vec3;

struct EulerAngles
{
public:
	float m_yawDegrees = 0.f;
	float m_pitchDegrees = 0.f;
	float m_rollDegrees = 0.f;

public:
	EulerAngles() = default;
	EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees);
	void GetAsVectors_XFwd_YLeft_ZUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis) const;
	Mat44 GetAsMatrix_XFwd_YLeft_ZUp() const;

	const EulerAngles operator*(float scaleFactor) const;
	void operator+=(const EulerAngles& angleToAdd);
	EulerAngles operator+(const EulerAngles& angleToAdd);

	void SetFromText(const char* text);
};