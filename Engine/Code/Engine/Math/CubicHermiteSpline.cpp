#include "Engine/Math/CubicHermiteSpline.hpp"
#include "Engine/Math/CubicHermiteCurve2D.hpp"

CubicHermiteSpline::CubicHermiteSpline(const std::vector<Vec2>& controlPositions)
	:m_controlPositions(controlPositions)
{
	for (int i = 0; i < m_controlPositions.size(); i++)
	{
		int next = i + 1;
		int prev = i - 1;
		if (next >= m_controlPositions.size() || prev < 0)
			m_positionVelocities.push_back(Vec2::ZERO);
		else
		{
			Vec2 velocity = (m_controlPositions[next] - m_controlPositions[prev]) / 2.f;
			m_positionVelocities.push_back(velocity);
		}
	}

	m_numCurves = (int)m_controlPositions.size() - 1;
}

Vec2 CubicHermiteSpline::EvaluateAtParametric(float t) const
{
	int curveToEvaluate = static_cast<int>(t);
	float parametricZeroToOneForTheCurve = t - (float)curveToEvaluate;
	return GetCubicHermiteCurve(curveToEvaluate).EvaluateAtParametric(parametricZeroToOneForTheCurve);
}

float CubicHermiteSpline::GetApproximateLength(int numSubDivisions /*= 64*/) const
{
	float totalLength = 0.f;
	for (int i = 0; i < m_numCurves; i++)
	{
		CubicHermiteCurve2D hermiteCurve = GetCubicHermiteCurve(i);
		totalLength += hermiteCurve.GetApproximateLength(numSubDivisions);
	}

	return totalLength;
}

Vec2 CubicHermiteSpline::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubDivisions /*= 64*/)
{
	float currentTotalLength = 0.f;
	for (int i = 0; i < m_numCurves; i++)
	{
		CubicHermiteCurve2D hermiteCurve = GetCubicHermiteCurve(i);
		float lenghtOfSubCurve = hermiteCurve.GetApproximateLength(numSubDivisions);
		if (currentTotalLength + lenghtOfSubCurve > distanceAlongCurve)
		{
			return hermiteCurve.EvaluateAtApproximateDistance(distanceAlongCurve - currentTotalLength, numSubDivisions);
		}
		currentTotalLength += lenghtOfSubCurve;
	}

	//if distance along curve is greater than spline length, return end point
	return EvaluateAtParametric(1.f);
}

CubicHermiteCurve2D CubicHermiteSpline::GetCubicHermiteCurve(int subCurve) const
{
	if (subCurve < 0 || subCurve >= m_controlPositions.size())
		return CubicHermiteCurve2D(Vec2::ZERO, Vec2::ZERO, Vec2::ZERO, Vec2::ZERO);

	return CubicHermiteCurve2D(m_controlPositions[subCurve], m_positionVelocities[subCurve], m_controlPositions[subCurve + 1], m_positionVelocities[subCurve + 1]);
}
