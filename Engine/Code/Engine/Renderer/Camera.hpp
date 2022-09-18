#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/AABB2.hpp"

enum class CameraMode
{
	ORTHOGRAPHIC,
	PERSPECTIVE
};

class Camera
{
public:
	Camera() {}

	Vec2 GetOrthoBottomLeft() const;
	Vec2 GetOrthoTopRight() const;
	Vec3 GetForward() const;
	Vec3 GetUpVector() const;
	Vec3 GetLeft() const;
	void GetTransform(Vec3& out_Positon, EulerAngles& out_orientation) const;
	AABB2 GetCameraViewport() const;
	float GetFOV() const;

	Mat44 GetOrthoMatrix() const;
	Mat44 GetPerspectiveMatrix() const;
	Mat44 GetProjectionMatrix() const;
	Mat44 GetViewMatrix() const;
	Mat44 GetViewToRenderMatrix() const;

	void SetOrthoView(Vec2 const& bottomLeftCorner, Vec2 const& topRightCorner);
	void SetPerspectiveView(float aspect, float fov, float nearPlaneDistance, float farPlaneDistance);
	void SetViewToRenderTransform(const Vec3& iBasis, const Vec3& jBasis, const Vec3& kBasis);
	void SetTransform(Vec3 newPosition, EulerAngles newOrientation);
	void SetFOV(float fov);
	void SetCameraViewport(const AABB2& viewport);
	
	void Translate(Vec2 translation);

protected:
	Vec2 m_botLeft;
	Vec2 m_topRight;
	float m_perspectivePlaneAspect = 0.f;
	float m_perspectiveCameraFov = 0.f;
	float m_perspectiveNearPlaneDistance = 0.f;
	float m_perspectiveFarPlaneDistance = 0.f;
	CameraMode m_mode = CameraMode::ORTHOGRAPHIC;
	Vec3 m_position;
	EulerAngles m_orientation;
	Mat44 m_viewToRenderMatrix;
	AABB2 m_viewport = AABB2(Vec2(0.f, 0.f), Vec2(1.f, 1.f));
};