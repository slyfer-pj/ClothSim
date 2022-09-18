#pragma once
#include "Engine/Math/Vec2.hpp"

class CubicHermiteCurve2D;

class CubicBezierCurve2D
{
public:
	Vec2 m_startPos = Vec2::ZERO;
	Vec2 m_guidePos1 = Vec2::ZERO;
	Vec2 m_guidePos2 = Vec2::ZERO;
	Vec2 m_endPos = Vec2::ZERO;

public:
	CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos);
	explicit CubicBezierCurve2D(const CubicHermiteCurve2D& fromHermite);
	Vec2 EvaluateAtParametric(float parametricZeroToOne) const;
	float GetApproximateLength(int numSubDivisions = 64) const;
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubDivisions = 64);
};