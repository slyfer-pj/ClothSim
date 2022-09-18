#include "Engine/Math/CubicBezierCurve2D.hpp"
#include "Engine/Math/CubicHermiteCurve2D.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

CubicBezierCurve2D::CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos)
	:m_startPos(startPos), m_guidePos1(guidePos1), m_guidePos2(guidePos2), m_endPos(endPos)
{
}

CubicBezierCurve2D::CubicBezierCurve2D(const CubicHermiteCurve2D& fromHermite)
	: m_startPos(fromHermite.m_startPos), m_endPos(fromHermite.m_endPos)
{
	m_guidePos1 = fromHermite.m_startPos + (fromHermite.m_startVelocity / 3.f);
	m_guidePos2 = fromHermite.m_endPos - (fromHermite.m_endVelocity / 3.f);
}

Vec2 CubicBezierCurve2D::EvaluateAtParametric(float parametricZeroToOne) const
{
	Vec2 ab = Interpolate(m_startPos, m_guidePos1, parametricZeroToOne);
	Vec2 bc = Interpolate(m_guidePos1, m_guidePos2, parametricZeroToOne);
	Vec2 cd = Interpolate(m_guidePos2, m_endPos, parametricZeroToOne);

	Vec2 abc = Interpolate(ab, bc, parametricZeroToOne);
	Vec2 bcd = Interpolate(bc, cd, parametricZeroToOne);

	Vec2 abcd = Interpolate(abc, bcd, parametricZeroToOne);
	return abcd;
}

float CubicBezierCurve2D::GetApproximateLength(int numSubDivisions /*= 64*/) const
{
	float length = 0.f;
	Vec2 pointA = m_startPos;
	Vec2 pointB = Vec2::ZERO;
	for (int i = 1; i <= numSubDivisions; i++)
	{
		float t = (float)i / (float)numSubDivisions;
		pointB = EvaluateAtParametric(t);
		length += GetDistance2D(pointA, pointB);
		pointA = pointB;
	}
	return length;
}

Vec2 CubicBezierCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubDivisions /*= 64*/)
{
	float currentLength = 0.f;
	Vec2 pointA = m_startPos;
	Vec2 pointB = Vec2::ZERO;
	for (int i = 1; i <= numSubDivisions; i++)
	{
		float t = (float)i / (float)numSubDivisions;
		pointB = EvaluateAtParametric(t);
		float subDivisionLength = GetDistance2D(pointA, pointB);
		if (currentLength + subDivisionLength > distanceAlongCurve)
		{
			float lengthFraction = (distanceAlongCurve - currentLength) / subDivisionLength;
			//DebuggerPrintf("dist = %f, frac = %f\n", distanceAlongCurve, lengthFraction);
			return Interpolate(pointA, pointB, lengthFraction);
			//return pointA + ((pointB - pointA).GetNormalized() * (currentLength - distanceAlongCurve));
		}
		currentLength += subDivisionLength;
		pointA = pointB;

	}

	return pointB;
}

