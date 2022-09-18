#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/MathUtils.hpp"

EulerAngles::EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees)
	:m_yawDegrees(yawDegrees), m_pitchDegrees(pitchDegrees), m_rollDegrees(rollDegrees)
{
}

void EulerAngles::GetAsVectors_XFwd_YLeft_ZUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis) const
{
	float cy = CosDegrees(m_yawDegrees);
	float sy = SinDegrees(m_yawDegrees);
	float cp = CosDegrees(m_pitchDegrees);
	float sp = SinDegrees(m_pitchDegrees);
	float cr = CosDegrees(m_rollDegrees);
	float sr = SinDegrees(m_rollDegrees);

	out_forwardIBasis.x = cy * cp;
	out_forwardIBasis.y = sy * cp;
	out_forwardIBasis.z = -sp;

	out_leftJBasis.x = (-sy * cr) + (cy * sp * sr);
	out_leftJBasis.y = (cy * cr) + (sy * sp * sr);
	out_leftJBasis.z = cp * sr;

	out_upKBasis.x = (sy * sr) + (cy * sp * cr);
	out_upKBasis.y = (-cy * sr) + (sy * sp * cr);
	out_upKBasis.z = cp * cr;
}

Mat44 EulerAngles::GetAsMatrix_XFwd_YLeft_ZUp() const
{
	Mat44 rotationMat = Mat44::CreateZRotationDegrees(m_yawDegrees);
	rotationMat.Append(Mat44::CreateYRotationDegrees(m_pitchDegrees));
	rotationMat.Append(Mat44::CreateXRotationDegrees(m_rollDegrees));
	return rotationMat;
}

const EulerAngles EulerAngles::operator*(float scaleFactor) const
{
	return EulerAngles(m_yawDegrees * scaleFactor, m_pitchDegrees * scaleFactor, m_rollDegrees * scaleFactor);
}

void EulerAngles::SetFromText(const char* text)
{
	Strings subStrings = SplitStringOnDelimiter(text, ',');
	m_yawDegrees = (float)atof(subStrings[0].c_str());
	m_pitchDegrees = (float)atof(subStrings[1].c_str());
	m_rollDegrees = (float)atof(subStrings[2].c_str());
}

void EulerAngles::operator+=(const EulerAngles& angleToAdd)
{
	m_yawDegrees += angleToAdd.m_yawDegrees;
	m_pitchDegrees += angleToAdd.m_pitchDegrees;
	m_rollDegrees += angleToAdd.m_rollDegrees;
}

EulerAngles EulerAngles::operator+(const EulerAngles& angleToAdd)
{
	return EulerAngles(m_yawDegrees + angleToAdd.m_yawDegrees, m_pitchDegrees + angleToAdd.m_pitchDegrees, m_rollDegrees + angleToAdd.m_rollDegrees);
}
