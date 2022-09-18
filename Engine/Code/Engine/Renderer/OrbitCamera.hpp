#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Renderer/Camera.hpp"

class OrbitCamera : public Camera
{
public:
	OrbitCamera() = default;
	explicit OrbitCamera(const Vec3& focalPoint, float radius);
	void SetFocalPoint(const Vec3& newFocalPoint);
	void SetRotation(const EulerAngles& newRotation);
	void UpdateOrientation(float deltaYaw, float deltaPitch);

private:
	void SetCameraPosition();
	void SetCameraOrientation();

private:
	Vec3 m_focalPoint;
	EulerAngles m_yawPitchRotation;
	float m_radius = 1.f;
};