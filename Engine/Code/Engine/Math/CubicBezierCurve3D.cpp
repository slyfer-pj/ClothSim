#include "Engine/Math/CubicBezierCurve3D.hpp"
#include "Engine/Math/MathUtils.hpp"

CubicBezierCurve3D::CubicBezierCurve3D(Vec3 startPos, Vec3 guidePos1, Vec3 guidePos2, Vec3 endPos)
	:m_startPos(startPos), m_guidePos1(guidePos1), m_guidePos2(guidePos2), m_endPos(endPos)
{

}

Vec3 CubicBezierCurve3D::EvaluateAtParametric(float parametricZeroToOne) const
{
	Vec3 ab = Interpolate(m_startPos, m_guidePos1, parametricZeroToOne);
	Vec3 bc = Interpolate(m_guidePos1, m_guidePos2, parametricZeroToOne);
	Vec3 cd = Interpolate(m_guidePos2, m_endPos, parametricZeroToOne);

	Vec3 abc = Interpolate(ab, bc, parametricZeroToOne);
	Vec3 bcd = Interpolate(bc, cd, parametricZeroToOne);

	Vec3 abcd = Interpolate(abc, bcd, parametricZeroToOne);
	return abcd;
}

float CubicBezierCurve3D::GetApproximateLength(int numSubDivisions /*= 64*/) const
{
	float length = 0.f;
	Vec3 pointA = m_startPos;
	Vec3 pointB = Vec3::ZERO;
	for (int i = 1; i <= numSubDivisions; i++)
	{
		float t = (float)i / (float)numSubDivisions;
		pointB = EvaluateAtParametric(t);
		length += GetDistance3D(pointA, pointB);
		pointA = pointB;
	}
	return length;
}

Vec3 CubicBezierCurve3D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubDivisions /*= 64*/)
{
	float currentLength = 0.f;
	Vec3 pointA = m_startPos;
	Vec3 pointB = Vec3::ZERO;
	for (int i = 1; i <= numSubDivisions; i++)
	{
		float t = (float)i / (float)numSubDivisions;
		pointB = EvaluateAtParametric(t);
		float subDivisionLength = GetDistance3D(pointA, pointB);
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
