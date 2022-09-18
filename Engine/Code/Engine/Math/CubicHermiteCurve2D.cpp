#include "Engine/Math/CubicHermiteCurve2D.hpp"
#include "Engine/Math/CubicBezierCurve2D.hpp"

CubicHermiteCurve2D::CubicHermiteCurve2D(Vec2 startPos, Vec2 startVelocity, Vec2 endPos, Vec2 endVelocity)
	:m_startPos(startPos), m_startVelocity(startVelocity), m_endPos(endPos), m_endVelocity(endVelocity)
{

}

CubicHermiteCurve2D::CubicHermiteCurve2D(const CubicBezierCurve2D& fromBezier)
{
	m_startPos = fromBezier.m_startPos;
	m_endPos = fromBezier.m_endPos;
	m_startVelocity = (fromBezier.m_guidePos1 - fromBezier.m_startPos) * 3;
	m_endVelocity = (fromBezier.m_endPos - fromBezier.m_guidePos2) * 3;
}

Vec2 CubicHermiteCurve2D::EvaluateAtParametric(float parametricZeroToOne) const
{
	return CubicBezierCurve2D(*this).EvaluateAtParametric(parametricZeroToOne);
}

float CubicHermiteCurve2D::GetApproximateLength(int numSubDivisions /*= 64*/) const
{
	return CubicBezierCurve2D(*this).GetApproximateLength(numSubDivisions);
}

Vec2 CubicHermiteCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, int numSubDivisions /*= 64*/)
{
	return CubicBezierCurve2D(*this).EvaluateAtApproximateDistance(distanceAlongCurve, numSubDivisions);
}
