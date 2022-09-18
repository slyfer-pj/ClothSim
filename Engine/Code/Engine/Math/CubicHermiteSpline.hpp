#pragma once
#include <vector>
#include "Engine/Math/Vec2.hpp"

class CubicHermiteCurve2D;

class CubicHermiteSpline
{
public:
	std::vector<Vec2> m_controlPositions;
	std::vector<Vec2> m_positionVelocities;
	int m_numCurves = 0;

public:
	CubicHermiteSpline(const std::vector<Vec2>& controlPositions);
	Vec2 EvaluateAtParametric(float t) const;
	float GetApproximateLength(int numSubDivisions = 64) const;
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubDivisions = 64);
	CubicHermiteCurve2D GetCubicHermiteCurve(int subCurve) const;
};