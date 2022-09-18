#include "Engine/Renderer/Camera.hpp"

constexpr float zNear = 0.f;
constexpr float zFar = 1.f;

void Camera::SetOrthoView(Vec2 const& bottomLeftCorner, Vec2 const& topRightCorner)
{
	m_botLeft = bottomLeftCorner;
	m_topRight = topRightCorner;
	m_mode = CameraMode::ORTHOGRAPHIC;
}

void Camera::SetPerspectiveView(float aspect, float fov, float nearPlaneDistance, float farPlaneDistance)
{
	m_perspectivePlaneAspect = aspect;
	m_perspectiveCameraFov = fov;
	m_perspectiveNearPlaneDistance = nearPlaneDistance;
	m_perspectiveFarPlaneDistance = farPlaneDistance;
	m_mode = CameraMode::PERSPECTIVE;
}

Vec2 Camera::GetOrthoBottomLeft() const
{
	return m_botLeft;
}

Vec2 Camera::GetOrthoTopRight() const
{
	return m_topRight;
}

Vec3 Camera::GetForward() const
{
	return m_orientation.GetAsMatrix_XFwd_YLeft_ZUp().GetIBasis3D();
}

Vec3 Camera::GetUpVector() const
{
	return m_orientation.GetAsMatrix_XFwd_YLeft_ZUp().GetKBasis3D();
}

Vec3 Camera::GetLeft() const
{
	return m_orientation.GetAsMatrix_XFwd_YLeft_ZUp().GetJBasis3D();
}

void Camera::Translate(Vec2 translation)
{
	m_botLeft += translation;
	m_topRight += translation;
}

Mat44 Camera::GetOrthoMatrix() const
{
	return Mat44::CreateOrthoProjection(m_botLeft.x, m_topRight.x, m_botLeft.y, m_topRight.y, zNear, zFar);
}

Mat44 Camera::GetPerspectiveMatrix() const
{
	return Mat44::CreatePerspectiveProjection(m_perspectiveCameraFov, m_perspectivePlaneAspect, m_perspectiveNearPlaneDistance, m_perspectiveFarPlaneDistance);
}

Mat44 Camera::GetProjectionMatrix() const
{
	Mat44 projectionMatrix;
	if (m_mode == CameraMode::ORTHOGRAPHIC)
		projectionMatrix = GetOrthoMatrix();
	else
		projectionMatrix = GetPerspectiveMatrix();

	projectionMatrix.Append(GetViewToRenderMatrix());
	return projectionMatrix;
}

void Camera::SetTransform(Vec3 newPosition, EulerAngles newOrientation)
{
	m_position = newPosition;
	m_orientation = newOrientation;
}

void Camera::GetTransform(Vec3& out_Positon, EulerAngles& out_orientation) const
{
	out_Positon = m_position;
	out_orientation = m_orientation;
}

Mat44 Camera::GetViewMatrix() const
{
	Mat44 camTranslationMat = Mat44::CreateTranslation3D(m_position);
	Mat44 camRotationMat = m_orientation.GetAsMatrix_XFwd_YLeft_ZUp();
	camTranslationMat.Append(camRotationMat);
	Mat44 viewMatrix = camTranslationMat.GetOrthonormalInverse();
	return viewMatrix;
}

void Camera::SetViewToRenderTransform(const Vec3& iBasis, const Vec3& jBasis, const Vec3& kBasis)
{
	m_viewToRenderMatrix = Mat44(iBasis, jBasis, kBasis, Vec3());
}

Mat44 Camera::GetViewToRenderMatrix() const
{
	return m_viewToRenderMatrix;
}

void Camera::SetFOV(float fov)
{
	m_perspectiveCameraFov = fov;
}

void Camera::SetCameraViewport(const AABB2& viewport)
{
	m_viewport = viewport;
}

AABB2 Camera::GetCameraViewport() const
{
	return m_viewport;
}

float Camera::GetFOV() const
{
	return m_perspectiveCameraFov;
}
