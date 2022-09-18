#include "Engine/Math/AABB3.hpp"

const AABB3 AABB3::ZERO_TO_ONE = AABB3(Vec3::ZERO, Vec3(1.f, 1.f, 1.f));

AABB3::AABB3(const AABB3& copyFrom)
	:m_mins(copyFrom.m_mins), m_maxs(copyFrom.m_maxs)
{
}

AABB3::AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	:m_mins(Vec3(minX, minY, minZ)), m_maxs(Vec3(maxX, maxY, maxZ))
{
}

AABB3::AABB3(const Vec3& mins, const Vec3& maxs)
	:m_mins(mins), m_maxs(maxs)
{
}

bool AABB3::IsPointInside(const Vec3& referencePoint)
{
	if (referencePoint.x > m_mins.x &&
		referencePoint.x < m_maxs.x &&
		referencePoint.y > m_mins.y &&
		referencePoint.y < m_maxs.y &&
		referencePoint.z > m_mins.z &&
		referencePoint.z < m_mins.z)
	{
		return true;
	}

	return false;
}

Vec3 AABB3::GetCenter() const
{
	float centerX = (m_mins.x + m_maxs.x) / 2;
	float centerY = (m_mins.y + m_maxs.y) / 2;
	float centerZ = (m_mins.z + m_maxs.z) / 2;
	return Vec3(centerX, centerY, centerZ);
}
