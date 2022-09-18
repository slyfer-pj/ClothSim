#pragma once
#include "Engine/Math/Vec2.hpp"

class CubicBezierCurve2D;

class CubicHermiteCurve2D
{
public:
	Vec2 m_startPos = Vec2::ZERO;
	Vec2 m_startVelocity = Vec2::ZERO;
	Vec2 m_endPos = Vec2::ZERO;
	Vec2 m_endVelocity = Vec2::ZERO;

public:
	CubicHermiteCurve2D(Vec2 startPos, Vec2 startVelocity, Vec2 endPos, Vec2 endVelocity);
	explicit CubicHermiteCurve2D(const CubicBezierCurve2D& fromBezier);
	Vec2 EvaluateAtParametric(float parametricZeroToOne) const;
	float GetApproximateLength(int numSubDivisions = 64) const;
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubDivisions = 64);
};