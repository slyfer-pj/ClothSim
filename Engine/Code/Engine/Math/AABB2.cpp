#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/MathUtils.hpp"

const AABB2 AABB2::ZERO_TO_ONE = AABB2(Vec2::ZERO, Vec2::ONE);

AABB2::AABB2(const AABB2& copyFrom)
	: m_mins(copyFrom.m_mins), m_maxs(copyFrom.m_maxs)
{
}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
	: m_mins(Vec2(minX,minY)), m_maxs(Vec2(maxX, maxY))
{

}

AABB2::AABB2(const Vec2& mins, const Vec2& maxs)
	: m_mins(mins), m_maxs(maxs)
{

}

bool AABB2::IsPointInside(const Vec2& point) const
{
	if (point.x > m_mins.x &&
		point.x < m_maxs.x &&
		point.y > m_mins.y &&
		point.y < m_maxs.y)
	{
		return true;
	}

	return false;
}

Vec2 AABB2::GetCenter() const
{
	float centerX = (m_mins.x + m_maxs.x) / 2;
	float centerY = (m_mins.y + m_maxs.y) / 2;
	return Vec2(centerX, centerY);
}

Vec2 AABB2::GetDimensions() const
{
	return Vec2(m_maxs.x - m_mins.x, m_maxs.y - m_mins.y);
}

Vec2 AABB2::GetNearestPoint(const Vec2& referencePoint) const
{
	float nearestX = referencePoint.x;
	float nearestY = referencePoint.y;

	if (nearestX < m_mins.x)
		nearestX = m_mins.x;
	else if (nearestX > m_maxs.x)
		nearestX = m_maxs.x;

	if (nearestY < m_mins.y)
		nearestY = m_mins.y;
	else if (nearestY > m_maxs.y)
		nearestY = m_maxs.y;

	return Vec2(nearestX, nearestY);
}

Vec2 AABB2::GetPointAtUV(const Vec2& uv) const
{
	float x = Interpolate(m_mins.x, m_maxs.x, uv.x);
	float y = Interpolate(m_mins.y, m_maxs.y, uv.y);

	return Vec2(x, y);
}

Vec2 AABB2::GetUVForPoint(const Vec2& point) const
{
	float u = GetFractionWithin(point.x, m_mins.x, m_maxs.x);
	float v = GetFractionWithin(point.y, m_mins.y, m_maxs.y);

	return Vec2(u, v);
}

AABB2 AABB2::GetBoxWithin(const AABB2& interiorBoxUV) const
{
	Vec2 mins = GetPointAtUV(interiorBoxUV.m_mins);
	Vec2 maxs = GetPointAtUV(interiorBoxUV.m_maxs);
	return AABB2(mins, maxs);
}

AABB2 AABB2::GetPaddedBox(float left, float right, float bottom, float top) const
{
	return AABB2(m_mins.x + left, m_mins.y + bottom, m_maxs.x - right, m_maxs.y - top);
}

void AABB2::Translate(const Vec2& translation)
{
	m_mins.x += translation.x;
	m_maxs.x += translation.x;
	m_mins.y += translation.y;
	m_maxs.y += translation.y;
}

void AABB2::SetCenter(const Vec2& newCenter)
{
	Vec2 translationVector = newCenter - GetCenter();
	Translate(translationVector);
}

void AABB2::SetDimensions(const Vec2& newDimensions)
{
	Vec2 center = GetCenter();
	m_mins.x = center.x - (newDimensions.x / 2);
	m_maxs.x = center.x + (newDimensions.x / 2);

	m_mins.y = center.y - (newDimensions.y / 2);
	m_maxs.y = center.y + (newDimensions.y / 2);
}

void AABB2::StretchToIncludePoint(const Vec2& pointToInclude)
{
	if (pointToInclude.x < m_mins.x)
		m_mins.x = pointToInclude.x;
	else if (pointToInclude.x > m_maxs.x)
		m_maxs.x = pointToInclude.x;

	if (pointToInclude.y < m_mins.y)
		m_mins.y = pointToInclude.y;
	else if (pointToInclude.y > m_maxs.y)
		m_maxs.y = pointToInclude.y;
}

