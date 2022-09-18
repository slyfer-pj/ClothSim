#pragma once
#include "Engine/Math/Vec3.hpp"

class CubicBezierCurve3D
{
public:
	Vec3 m_startPos = Vec3::ZERO;
	Vec3 m_guidePos1 = Vec3::ZERO;
	Vec3 m_guidePos2 = Vec3::ZERO;
	Vec3 m_endPos = Vec3::ZERO;

public:
	CubicBezierCurve3D(Vec3 startPos, Vec3 guidePos1, Vec3 guidePos2, Vec3 endPos);
	//explicit CubicBezierCurve2D(const CubicHermiteCurve2D& fromHermite);
	Vec3 EvaluateAtParametric(float parametricZeroToOne) const;
	float GetApproximateLength(int numSubDivisions = 64) const;
	Vec3 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubDivisions = 64);
};