#pragma once
#include "Engine/Math/Vec3.hpp"

struct AABB3
{
public:
	Vec3 m_mins;
	Vec3 m_maxs;

	static const AABB3 ZERO_TO_ONE;

public:
	AABB3() {}
	~AABB3() {}
	AABB3(const AABB3& copyFrom);
	explicit AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
	explicit AABB3(const Vec3& mins, const Vec3& maxs);

	bool IsPointInside(const Vec3& referencePoint);
	Vec3 GetCenter() const;
};