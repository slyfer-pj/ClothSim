#pragma once
#include "Engine/Math/Vec2.hpp"

struct AABB2
{
public:
	Vec2 m_mins;
	Vec2 m_maxs;

	static const AABB2 ZERO_TO_ONE;

public:
	AABB2() {}
	~AABB2() {}
	AABB2(const AABB2& copyFrom);
	explicit AABB2(float minX, float minY, float maxX, float maxY);
	explicit AABB2(const Vec2& mins, const Vec2& maxs);

	bool IsPointInside(const Vec2& point) const;
	Vec2 GetCenter() const;
	Vec2 GetDimensions() const;
	Vec2 GetNearestPoint(const Vec2& referencePoint) const;
	Vec2 GetPointAtUV(const Vec2& uv) const;
	Vec2 GetUVForPoint(const Vec2& point) const;
	AABB2 GetBoxWithin(const AABB2& interiorBoxUV) const;
	AABB2 GetPaddedBox(float left, float right, float bottom, float top) const;

	void Translate(const Vec2& translation);
	void SetCenter(const Vec2& newCenter);
	void SetDimensions(const Vec2& newDimensions);
	void StretchToIncludePoint(const Vec2& pointToInclude);

};