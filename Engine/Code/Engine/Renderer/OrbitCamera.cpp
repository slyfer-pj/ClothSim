#include "Engine/Renderer/OrbitCamera.hpp"
#include "Engine/Math/MathUtils.hpp"

OrbitCamera::OrbitCamera(const Vec3& focalPoint, float radius)
	:m_focalPoint(focalPoint), m_radius(radius)
{
	SetCameraPosition();
	SetCameraOrientation();
}

void OrbitCamera::SetFocalPoint(const Vec3& newFocalPoint)
{
	m_focalPoint = newFocalPoint;

	SetCameraPosition();
	SetCameraOrientation();
}

void OrbitCamera::SetRotation(const EulerAngles& newRotation)
{
	m_yawPitchRotation = newRotation;

	SetCameraOrientation();
	SetCameraPosition();
}

void OrbitCamera::UpdateOrientation(float deltaYaw, float deltaPitch)
{
	m_yawPitchRotation.m_yawDegrees += deltaYaw;
	m_yawPitchRotation.m_pitchDegrees = Clamp(m_yawPitchRotation.m_pitchDegrees + deltaPitch, -80.f, 80.f);

	SetCameraPosition();
	SetCameraOrientation();
}

void OrbitCamera::SetCameraPosition()
{
	float cy = CosDegrees(m_yawPitchRotation.m_yawDegrees);
	float sy = SinDegrees(m_yawPitchRotation.m_yawDegrees);
	float cp = CosDegrees(m_yawPitchRotation.m_pitchDegrees);
	float sp = SinDegrees(m_yawPitchRotation.m_pitchDegrees);

	Vec3 displacement = Vec3(cy * cp, sy * cp, -sp) * m_radius;
	m_position = m_focalPoint + displacement;
}


void OrbitCamera::SetCameraOrientation()
{
	Vec3 cameraToFocalPoint = (m_focalPoint - m_position).GetNormalized();
	m_orientation.m_yawDegrees = cameraToFocalPoint.GetAngleAboutZDegrees();
	m_orientation.m_pitchDegrees = cameraToFocalPoint.GetAngleAboutYDegrees();
}

