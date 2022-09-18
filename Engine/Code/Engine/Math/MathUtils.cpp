#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <cmath>
#include <algorithm>

constexpr float PI = 3.14159f;

void TransformPosition2D(Vec2& positionToTransform, float uniformScaleXY, float rotationDegreeAboutZ, const Vec2& translationXY)
{
	float x = positionToTransform.x;
	float y = positionToTransform.y;

	//scaling
	x *= uniformScaleXY;
	y *= uniformScaleXY;

	//rotating
	float r = sqrtf((x * x) + (y * y));
	float thetaDegree = atan2f(y, x) * (180.f / PI);
	thetaDegree += rotationDegreeAboutZ;
	x = r * CosDegrees(thetaDegree);
	y = r * SinDegrees(thetaDegree);

	//translating
	x += translationXY.x;
	y += translationXY.y;

	positionToTransform.x = x;
	positionToTransform.y = y;
}

void TransformPosition2D(Vec2& positionToTransform, const Vec2& iBasis, const Vec2& jBasis, const Vec2& translationXY)
{
	positionToTransform = translationXY + (iBasis * positionToTransform.x) + (jBasis * positionToTransform.y);
}

void TransformPositionXY3D(Vec3& positionToTransform, float uniformScaleXY, float rotationDegreeAboutZ, const Vec2& translationXY)
{
	float x = positionToTransform.x;
	float y = positionToTransform.y;

	//scaling
	x *= uniformScaleXY;
	y *= uniformScaleXY;

	//rotating
	float r = sqrtf((x * x) + (y * y));
	float thetaDegree = atan2f(y, x) * (180.f / PI);
	thetaDegree += rotationDegreeAboutZ;
	x = r * CosDegrees(thetaDegree);
	y = r * SinDegrees(thetaDegree);

	//translating
	x += translationXY.x;
	y += translationXY.y;

	positionToTransform.x = x;
	positionToTransform.y = y;
}

void TransformPositionXY3D(Vec3& positionToTransform, const Vec2& iBasis, const Vec2& jBasis, const Vec2& translationXY)
{
	Vec2 transform = translationXY + (iBasis * positionToTransform.x) + (jBasis * positionToTransform.y);
	positionToTransform = Vec3(transform.x, transform.y, positionToTransform.z);
}

float GetFractionWithin(float inputValue, float inputRangeStart, float inputRangeEnd)
{
	if (inputRangeStart == inputRangeEnd)
		return 0.5f;

	float range = inputRangeEnd - inputRangeStart;
	return (inputValue - inputRangeStart) / range;
}

//float Interpolate(float start, float end, float fraction)
//{
//	return start + ((end - start) * fraction);
//}

float RangeMap(float inputValue, float inputRangeStart, float inputRangeEnd, float outputRangeStart, float outputRangeEnd)
{
	float fraction = GetFractionWithin(inputValue, inputRangeStart, inputRangeEnd);
	return Interpolate(outputRangeStart, outputRangeEnd, fraction);
}

float RangeMapClamped(float inputValue, float inputRangeStart, float inputRangeEnd, float outputRangeStart, float outputRangeEnd)
{
	float mappedValue = RangeMap(inputValue, inputRangeStart, inputRangeEnd, outputRangeStart, outputRangeEnd);

	if (outputRangeStart < outputRangeEnd)
		return Clamp(mappedValue, outputRangeStart, outputRangeEnd);
	else
		return Clamp(mappedValue, outputRangeEnd, outputRangeStart);
}

float ClampZeroToOne(float value)
{
	if (value < 0.f)
		return 0.f;
	else if (value > 1.f)
		return 1.f;

	return value;
}

int RoundDownToInt(float value)
{
	return static_cast<int>(floorf(value));
}

float GetShortestAngularDispDegrees(float fromDegrees, float toDegrees)
{
	float angularDispDegrees = toDegrees - fromDegrees;
	int numberOfWindings = ((int)angularDispDegrees) / 360;

	angularDispDegrees -= numberOfWindings * 360.f;

	if (angularDispDegrees > 180.f)
		angularDispDegrees -= 360.f;
	if (angularDispDegrees < -180.f)
		angularDispDegrees += 360.f;
	
	return angularDispDegrees;
}

float GetTurnedTowardDegrees(float fromDegrees, float toDegrees, float maxDeltaDegrees)
{
	float angularDispDegrees = GetShortestAngularDispDegrees(fromDegrees, toDegrees);

	if (fabsf(angularDispDegrees) <= maxDeltaDegrees)
		return toDegrees;
	else
	{
		if (angularDispDegrees < 0.f)
			return fromDegrees - maxDeltaDegrees;
		else
			return fromDegrees + maxDeltaDegrees;
	}
}

RaycastHit2D RaycastVsDisc(const Vec2& rayOrigin, const Vec2& rayForwardNormal, float rayMaxLength, const Vec2& discCenter, float discRadius)
{
	RaycastHit2D hit;

	Vec2 vectorFromLineStartToDiscCenter = discCenter - rayOrigin;
	Vec2 iBasisOfRaycastLine = rayForwardNormal;
	Vec2 jBasisOfRaycastLine = iBasisOfRaycastLine.GetRotated90Degrees();
	float jlengthFromRayToCenter = DotProduct2D(vectorFromLineStartToDiscCenter, jBasisOfRaycastLine);
	float ilengthFromRayToCenter = DotProduct2D(vectorFromLineStartToDiscCenter, iBasisOfRaycastLine);

	if (fabsf(jlengthFromRayToCenter) > discRadius)
		return hit;

	if (fabsf(ilengthFromRayToCenter) > (rayMaxLength + discRadius))
		return hit;

	if (ilengthFromRayToCenter < (0.f - discRadius))
		return hit;

	Vec2 nearestPointOnRayFromDiscCenter = GetNearestPointOnLineSegment2D(discCenter, rayOrigin, rayOrigin + (rayMaxLength * rayForwardNormal));
	if (!IsPointInsideDisc2D(nearestPointOnRayFromDiscCenter, discCenter, discRadius))
		return hit;

	hit.m_didImpact = true;
	if (GetDistanceSquared2D(rayOrigin, discCenter) > discRadius * discRadius)
	{
		float distFromImpactToPerpendicularOnRay = sqrt((discRadius * discRadius) - (jlengthFromRayToCenter * jlengthFromRayToCenter));
		float distanceOfPointOfImpactFromLineStart = ilengthFromRayToCenter - distFromImpactToPerpendicularOnRay;
		Vec2 pointOfImpact = rayOrigin + (distanceOfPointOfImpactFromLineStart * iBasisOfRaycastLine);
		hit.m_impactPosition = pointOfImpact;
		hit.m_impactDistance = distanceOfPointOfImpactFromLineStart;
		hit.m_impactNormal = (pointOfImpact - discCenter).GetNormalized();
	}
	else
	{
		hit.m_impactPosition = rayOrigin;
		hit.m_impactDistance = 0.f;
		hit.m_impactNormal = -rayForwardNormal;
	}

	return hit;
}


RaycastHit2D RaycastVsAABB2(const Vec2& rayOrigin, const Vec2& rayForwardNormal, float rayMaxLength, AABB2 bounds)
{
	RaycastHit2D hit;

	if (bounds.IsPointInside(rayOrigin))
	{
		hit.m_didImpact = true;
		hit.m_impactDistance = 0.f;
		hit.m_impactPosition = rayOrigin;
		hit.m_impactNormal = -rayForwardNormal;
		return hit;
	}

	float tEnterX = (bounds.m_mins.x - rayOrigin.x) / (rayForwardNormal.x * rayMaxLength);
	float tExitX = (bounds.m_maxs.x - rayOrigin.x) / (rayForwardNormal.x * rayMaxLength);
	float tEnterY = (bounds.m_mins.y - rayOrigin.y) / (rayForwardNormal.y * rayMaxLength);
	float tExitY = (bounds.m_maxs.y - rayOrigin.y) / (rayForwardNormal.y * rayMaxLength);

	//set the floatranges depending on if the ray is traveling from left/right or upwards/downwards
	FloatRange tRangeX = tEnterX < tExitX ? FloatRange(tEnterX, tExitX) : FloatRange(tExitX, tEnterX);
	FloatRange tRangeY = tEnterY < tExitY ? FloatRange(tEnterY, tExitY) : FloatRange(tExitY, tEnterY);

	if (tRangeX.IsOverlappingWith(tRangeY))
	{
		//calculate t
		float t = tRangeX.m_min;
		hit.m_impactNormal = Vec2(rayOrigin.x - bounds.m_mins.x, 0.f).GetNormalized();
		if (tRangeY.m_min > tRangeX.m_min)
		{
			t = tRangeY.m_min;
			hit.m_impactNormal = Vec2(0.f, rayOrigin.y - bounds.m_mins.y).GetNormalized();
		}

		//if t is not between (0, 1), no raycast hit
		if(t > 1.f || t < 0.f)
			return hit;

		hit.m_didImpact = true;
		hit.m_impactPosition = rayOrigin + (rayForwardNormal * t * rayMaxLength);
		hit.m_impactDistance = t * rayMaxLength;
	}

	return hit;
}

RaycastHit3D RaycastVsSphere3D(const Vec3& rayOrigin, const Vec3& rayForwardNormal, float rayMaxLength, const Vec3& sphereCenter, float sphereRadius)
{
	RaycastHit3D hit;

	Vec3 vectorFromLineStartToCenter = sphereCenter - rayOrigin;
	Vec3 iBasisOfRaycastLine = rayForwardNormal;
	float ilengthFromRayToCenter = DotProduct3D(vectorFromLineStartToCenter, iBasisOfRaycastLine);
	Vec3 jBasisOfRaycastLine = vectorFromLineStartToCenter - (iBasisOfRaycastLine * ilengthFromRayToCenter);
	float jlengthFromRayToCenter = jBasisOfRaycastLine.GetLength();

	if (fabsf(jlengthFromRayToCenter) > sphereRadius)
		return hit;

	if (fabsf(ilengthFromRayToCenter) > (rayMaxLength + sphereRadius))
		return hit;

	if (ilengthFromRayToCenter < (0.f - sphereRadius))
		return hit;

	hit.m_didImpact = true;
	if (GetDistanceSquared3D(rayOrigin, sphereCenter) > sphereRadius * sphereRadius)
	{
		float distanceFromImpactToPerpendicularOnRaycast = sqrt((sphereRadius * sphereRadius) - (jlengthFromRayToCenter * jlengthFromRayToCenter));
		hit.m_impactDistance = ilengthFromRayToCenter - distanceFromImpactToPerpendicularOnRaycast;
		hit.m_impactPosition = rayOrigin + (rayForwardNormal * hit.m_impactDistance);
		hit.m_impactNormal = (hit.m_impactPosition - sphereCenter).GetNormalized();
	}
	else
	{
		hit.m_impactDistance = 0.f;
		hit.m_impactPosition = rayOrigin;
		hit.m_impactNormal = -rayForwardNormal;
	}

	return hit;
}

RaycastHit3D RaycastVsAABB3(const Vec3& rayOrigin, const Vec3& rayForwardNormal, float rayMaxLength, AABB3 bounds)
{
	RaycastHit3D hit;

	if (bounds.IsPointInside(rayOrigin))
	{
		hit.m_didImpact = true;
		hit.m_impactDistance = 0.f;
		hit.m_impactPosition = rayOrigin;
		hit.m_impactNormal = -rayForwardNormal;
		return hit;
	}

	float tEnterX = (bounds.m_mins.x - rayOrigin.x) / (rayForwardNormal.x * rayMaxLength);
	float tExitX = (bounds.m_maxs.x - rayOrigin.x) / (rayForwardNormal.x * rayMaxLength);
	float tEnterY = (bounds.m_mins.y - rayOrigin.y) / (rayForwardNormal.y * rayMaxLength);
	float tExitY = (bounds.m_maxs.y - rayOrigin.y) / (rayForwardNormal.y * rayMaxLength);
	float tEnterZ = (bounds.m_mins.z - rayOrigin.z) / (rayForwardNormal.z * rayMaxLength);
	float tExitZ = (bounds.m_maxs.z - rayOrigin.z) / (rayForwardNormal.z * rayMaxLength);

	//set the floatranges depending on if the ray is traveling from left/right or upwards/downwards
	FloatRange tRangeX = tEnterX < tExitX ? FloatRange(tEnterX, tExitX) : FloatRange(tExitX, tEnterX);
	FloatRange tRangeY = tEnterY < tExitY ? FloatRange(tEnterY, tExitY) : FloatRange(tExitY, tEnterY);
	FloatRange tRangeZ = tEnterZ < tExitZ ? FloatRange(tEnterZ, tExitZ) : FloatRange(tExitZ, tEnterZ);

	//if all of the tEnters are out of range 0 and 1, the ray doesn't hit
	if ((tRangeX.m_min < 0.f || tRangeX.m_min > 1.f) && (tRangeY.m_min < 0.f || tRangeY.m_min > 1.f) && (tRangeZ.m_min < 0.f || tRangeZ.m_min > 1.f))
		return hit;

	if (tRangeX.IsOverlappingWith(tRangeY) && tRangeY.IsOverlappingWith(tRangeZ) && tRangeZ.IsOverlappingWith(tRangeX))
	{
		//raycast hit
		hit.m_didImpact = true;
		float t = tRangeX.m_min;
		hit.m_impactNormal = Vec3(rayOrigin.x - bounds.m_mins.x, 0.f, 0.f).GetNormalized();
		if (tRangeY.m_min > t)
		{
			t = tRangeY.m_min;
			hit.m_impactNormal = Vec3(0.f, rayOrigin.y - bounds.m_mins.y, 0.f).GetNormalized();
		}
		if (tRangeZ.m_min > t)
		{
			t = tRangeZ.m_min;
			hit.m_impactNormal = Vec3(0.f, 0.f, rayOrigin.z - bounds.m_mins.z).GetNormalized();
		}
		hit.m_impactPosition = rayOrigin + (rayForwardNormal * t * rayMaxLength);
		hit.m_impactDistance = t * rayMaxLength;
	}

	return hit;
}

RaycastHit3D RaycastVsZCylinder(const Vec3& rayOrigin, const Vec3& rayForwardNormal, float rayMaxLength, const Vec2& zCylinderXYCenter, float zCylinderRadius, FloatRange zCylinderHeightRange)
{
	RaycastHit3D result;
	Vec2 discRaycastStart = Vec2(rayOrigin.x, rayOrigin.y);
	Vec2 discRaycastDirection = Vec2(rayForwardNormal.x, rayForwardNormal.y).GetNormalized();
	Vec2 discRaycastEnd = discRaycastStart + (rayMaxLength * discRaycastDirection);
	Vec2 discCenter = zCylinderXYCenter;
	Vec3 rayEnd = rayOrigin + rayForwardNormal * rayMaxLength;
	RaycastHit2D discHit = RaycastVsDisc(discRaycastStart, discRaycastDirection, rayMaxLength, discCenter, zCylinderRadius);
	if (discHit.m_didImpact)
	{
		//check if raycast start is inside cylinder. if so return start as impact
		if ((discCenter - discRaycastStart).GetLengthSquared() < (zCylinderRadius * zCylinderRadius) &&
			rayOrigin.z > zCylinderHeightRange.m_min && rayOrigin.z < zCylinderHeightRange.m_max)
		{
			result.m_didImpact = true;
			result.m_impactPosition = rayOrigin;
			result.m_impactNormal = -rayForwardNormal;
			return result;
		}

		//check if ray is hitting from top
		if (rayOrigin.z > zCylinderHeightRange.m_max)
		{
			if (rayForwardNormal.z > 0.f)
				return result;

			float tImpactTopFace = (rayOrigin.z - zCylinderHeightRange.m_max) / fabsf(rayForwardNormal.z * rayMaxLength);
			Vec3 pointOfImpactTop = rayOrigin + (tImpactTopFace * rayForwardNormal * rayMaxLength);
			if (tImpactTopFace > 0.f && tImpactTopFace < 1.f &&
				IsPointInsideDisc2D(Vec2(pointOfImpactTop.x, pointOfImpactTop.y), discCenter, zCylinderRadius))
			{
				result.m_didImpact = true;
				result.m_impactPosition = pointOfImpactTop;
				result.m_impactDistance = tImpactTopFace * rayMaxLength;
				result.m_impactNormal = Vec3(0.f, 0.f, 1.f);
				return result;
			}
		}
		//check if ray is hitting from bottom
		else if (rayOrigin.z < zCylinderHeightRange.m_min)
		{
			if (rayForwardNormal.z < 0.f)
				return result;

			float tImpactBotFace = (zCylinderHeightRange.m_min - rayOrigin.z) / fabsf(rayForwardNormal.z * rayMaxLength);
			Vec3 pointOfImpactBot = rayOrigin + (tImpactBotFace * rayForwardNormal * rayMaxLength);
			if (tImpactBotFace > 0.f && tImpactBotFace < 1.f &&
				IsPointInsideDisc2D(Vec2(pointOfImpactBot.x, pointOfImpactBot.y), discCenter, zCylinderRadius))
			{
				result.m_didImpact = true;
				result.m_impactPosition = pointOfImpactBot;
				result.m_impactDistance = tImpactBotFace * rayMaxLength;
				result.m_impactNormal = Vec3(0.f, 0.f, -1.f);
				return result;
			}
		}

		//ray is hitting on the side
		float tImpactOnSide = (discHit.m_impactPosition - discRaycastStart).GetLength() / (Vec2(rayForwardNormal.x, rayForwardNormal.y).GetLength() * rayMaxLength);
		Vec3 pointOfImpactSide = rayOrigin + (tImpactOnSide * rayForwardNormal * rayMaxLength);
		if (pointOfImpactSide.z > zCylinderHeightRange.m_max || pointOfImpactSide.z < zCylinderHeightRange.m_min)
			return result;

		result.m_didImpact = true;
		result.m_impactPosition = pointOfImpactSide;
		result.m_impactDistance = tImpactOnSide * rayMaxLength;
		result.m_impactNormal = (pointOfImpactSide - Vec3(zCylinderXYCenter.x, zCylinderXYCenter.y, pointOfImpactSide.z)).GetNormalized();
		return result;

	}

	return result;
}

RaycastHit2D RaycastVsLineSegment(const Vec2& rayOrigin, const Vec2& rayForwardNormal, float rayMaxLength, const Vec2& lineSegStart, const Vec2& lineSegEnd)
{
	//R = rayorigin, S = linestart, E = lineEnd, i = ray fwd, j = perpendicular of i
	RaycastHit2D result;
	Vec2 vecSE = lineSegEnd - lineSegStart;
	Vec2 j = rayForwardNormal.GetRotated90Degrees();
	Vec2 vecRS = lineSegStart - rayOrigin;
	Vec2 vecRE = lineSegEnd - rayOrigin;
	float RSi = DotProduct2D(vecRS, rayForwardNormal);
	float REi = DotProduct2D(vecRE, rayForwardNormal);
	float RSj = DotProduct2D(vecRS, j);
	float REj = DotProduct2D(vecRE, j);
	if (RSj * REj > 0)		//both the start and end are on the same side of the ray, so its a miss
		return result;
	if (RSi < 0.f && REi < 0.f) // both start and end are before the ray
		return result;
	if (RSi > rayMaxLength && REi > rayMaxLength) //both start and end are after the ray max
		return result;

	float impactFraction = RSj / (RSj + (-REj));	//using similar triangles
	Vec2 impactPoint = lineSegStart + (impactFraction * vecSE);
	Vec2 vecRI = impactPoint - rayOrigin;
	float impactDistance = DotProduct2D(vecRI, rayForwardNormal);
	if (impactDistance < 0.f || impactDistance > rayMaxLength)
		return result;
	Vec2 impactNormal = vecSE.GetRotated90Degrees().GetNormalized();
	if (RSj > 0.f)
		impactNormal = -impactNormal;

	result.m_didImpact = true;
	result.m_impactDistance = impactDistance;
	result.m_impactNormal = impactNormal;
	result.m_impactPosition = impactPoint;
	return result;
}

RaycastHit2D RaycastVsOBB2(const Vec2& rayOrigin, const Vec2& rayForwardNormal, float rayMaxLength, const OBB2& orientedBox)
{
	//convert rayorigin and rayForward to obb local space
	Vec2 dispFromCenterToRayOrigin = rayOrigin - orientedBox.m_center;
	Vec2 rayOriginInOBBLocalSpace = Vec2(GetProjectedLength2D(dispFromCenterToRayOrigin, orientedBox.m_iBasisNormal), 
		GetProjectedLength2D(dispFromCenterToRayOrigin, orientedBox.m_iBasisNormal.GetRotated90Degrees()));
	Vec2 rayForwardInOBBLocalSpace = Vec2(DotProduct2D(rayForwardNormal, orientedBox.m_iBasisNormal), DotProduct2D(rayForwardNormal, orientedBox.m_iBasisNormal.GetRotated90Degrees()));

	//proceed to to raycast vs aabb
	RaycastHit2D hit;
	AABB2 bounds = AABB2(-orientedBox.m_halfDimensions, orientedBox.m_halfDimensions);

	if (bounds.IsPointInside(rayOriginInOBBLocalSpace))
	{
		hit.m_didImpact = true;
		hit.m_impactDistance = 0.f;
		hit.m_impactPosition = rayOrigin;
		hit.m_impactNormal = -rayForwardNormal;
		return hit;
	}

	float tEnterX = (bounds.m_mins.x - rayOriginInOBBLocalSpace.x) / (rayForwardInOBBLocalSpace.x * rayMaxLength);
	float tExitX = (bounds.m_maxs.x - rayOriginInOBBLocalSpace.x) / (rayForwardInOBBLocalSpace.x * rayMaxLength);
	float tEnterY = (bounds.m_mins.y - rayOriginInOBBLocalSpace.y) / (rayForwardInOBBLocalSpace.y * rayMaxLength);
	float tExitY = (bounds.m_maxs.y - rayOriginInOBBLocalSpace.y) / (rayForwardInOBBLocalSpace.y * rayMaxLength);

	//set the floatranges depending on if the ray is traveling from left/right or upwards/downwards
	FloatRange tRangeX = tEnterX < tExitX ? FloatRange(tEnterX, tExitX) : FloatRange(tExitX, tEnterX);
	FloatRange tRangeY = tEnterY < tExitY ? FloatRange(tEnterY, tExitY) : FloatRange(tExitY, tEnterY);

	if (tRangeX.IsOverlappingWith(tRangeY))
	{
		//calculate t
		float t = tRangeX.m_min;
		hit.m_impactNormal = Vec2(rayOriginInOBBLocalSpace.x - bounds.m_mins.x, 0.f).GetNormalized();
		if (tRangeY.m_min > tRangeX.m_min)
		{
			t = tRangeY.m_min;
			hit.m_impactNormal = Vec2(0.f, rayOriginInOBBLocalSpace.y - bounds.m_mins.y).GetNormalized();
		}

		//if t is not between (0, 1), no raycast hit
		if (t > 1.f || t < 0.f)
			return hit;

		hit.m_didImpact = true;
		hit.m_impactPosition = rayOrigin + (rayForwardNormal * t * rayMaxLength);
		hit.m_impactDistance = t * rayMaxLength;
	}

	return hit;
}

float NormalizeByte(unsigned char byteValue)
{
	float inputValue = static_cast<float>(byteValue);
	float retValue = RangeMapClamped(inputValue, 0.f, 255.f, 0.f, 1.f);
	return retValue;
}

unsigned char DenormalizeByte(float zeroToOne)
{
	float denomalizedByte = floor(zeroToOne >= 1.f ? 255.f : zeroToOne * 256.f);
	return static_cast<unsigned char>(denomalizedByte);
}

float SmoothStart2(float t)
{
	return (t * t);
}

float SmoothStart3(float t)
{
	return (t * t * t);
}

float SmoothStart4(float t)
{
	return (t * t * t * t);
}

float SmoothStart5(float t)
{
	return (t * t * t * t * t);
}

float SmoothStart6(float t)
{
	return (t * t * t * t * t * t);
}

float SmoothStop2(float t)
{
	float oneMinusT = (1.f - t);
	return 1.f - (oneMinusT * oneMinusT);
}

float SmoothStop3(float t)
{
	float oneMinusT = (1.f - t);
	return 1.f - (oneMinusT * oneMinusT * oneMinusT);
}

float SmoothStop4(float t)
{
	float oneMinusT = (1.f - t);
	return 1.f - (oneMinusT * oneMinusT * oneMinusT * oneMinusT);
}

float SmoothStop5(float t)
{
	float oneMinusT = (1.f - t);
	return 1.f - (oneMinusT * oneMinusT * oneMinusT * oneMinusT * oneMinusT);
}

float SmoothStop6(float t)
{
	float oneMinusT = (1.f - t);
	return 1.f - (oneMinusT * oneMinusT * oneMinusT * oneMinusT * oneMinusT * oneMinusT);
}

float SmoothStep3(float t)
{
	return ComputeCubicBezier1D(0.f, 0.f, 1.f, 1.f, t);
}

float SmoothStep5(float t)
{
	return ComputeQuinticBezier1D(0.f, 0.f, 0.f, 1.f, 1.f, 1.f, t);
}

float Hesitate3(float t)
{
	return ComputeCubicBezier1D(0.f, 1.f, 0.f, 1.f, t);
}

float Hesitate5(float t)
{
	return ComputeQuinticBezier1D(0.f, 1.f, 0.f, 1.f, 0.f, 1.f, t);
}

float ComputeCubicBezier1D(float A, float B, float C, float D, float t)
{
	float ab = Interpolate(A, B, t);
	float bc = Interpolate(B, C, t);
	float cd = Interpolate(C, D, t);

	float abc = Interpolate(ab, bc, t);
	float bcd = Interpolate(bc, cd, t);

	float abcd = Interpolate(abc, bcd, t);
	return abcd;
}

float ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t)
{
	float ab = Interpolate(A, B, t);
	float bc = Interpolate(B, C, t);
	float cd = Interpolate(C, D, t);
	float de = Interpolate(D, E, t);
	float ef = Interpolate(E, F, t);

	float abc = Interpolate(ab, bc, t);
	float bcd = Interpolate(bc, cd, t);
	float cde = Interpolate(cd, de, t);
	float def = Interpolate(de, ef, t);

	float abcd = Interpolate(abc, bcd, t);
	float bcde = Interpolate(bcd, cde, t);
	float cdef = Interpolate(cde, def, t);

	float abcde = Interpolate(abcd, bcde, t);
	float bcdef = Interpolate(bcde, cdef, t);

	float abcdef = Interpolate(abcde, bcdef, t);
	return abcdef;
}

float DotProduct2D(const Vec2& a, const Vec2& b)
{
	return ((a.x * b.x) + (a.y * b.y));
}

float DotProduct3D(const Vec3& a, const Vec3& b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

float DotProduct4D(const Vec4& a, const Vec4& b)
{
	return ((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

float CrossProduct2D(const Vec2& a, const Vec2& b)
{
	return (a.x * b.y) - (a.y * b.x);
}

Vec3 CrossProduct3D(const Vec3& a, const Vec3& b)
{
	float i = (a.y * b.z) - (a.z * b.y);
	float j = (a.z * b.x) - (a.x * b.z);
	float k = (a.x * b.y) - (a.y * b.x);
	return Vec3(i, j, k);
}

float GetProjectedLength2D(const Vec2& vectorToProject, const Vec2& vectorToProjectOnto)
{
	return DotProduct2D(vectorToProject, vectorToProjectOnto.GetNormalized());
}

const Vec2 GetProjectedOnto2D(const Vec2& vectorToProject, const Vec2& vectorToProjectOnto)
{
	float projectedLength = GetProjectedLength2D(vectorToProject, vectorToProjectOnto);
	return vectorToProjectOnto.GetNormalized() * projectedLength;
}

float GetAngleDegreesBetweenVectors2D(const Vec2& a, const Vec2& b)
{
	float dotProduct = DotProduct2D(a.GetNormalized(), b.GetNormalized());
	return ConvertRadiansToDegrees(acosf(dotProduct));
}

float GetAngleDegreesBetweenVectors3D(const Vec3& a, const Vec3& b)
{
	float dotProduct = DotProduct3D(a.GetNormalized(), b.GetNormalized());
	return ConvertRadiansToDegrees(acosf(dotProduct));
}

bool CheckIfDiscOverlapsWithLine(const Vec2& discCenter, float discRadius, const Vec2& lineStart, const Vec2& lineEnd)
{
	const float lineCircleRadius = 0.1f;
	Vec2 currentCircleOnLineCenter = lineStart;
	Vec2 centerDisplacementEachIteration = (lineEnd - lineStart).GetNormalized() * lineCircleRadius;

	while (DotProduct2D(lineEnd - currentCircleOnLineCenter, centerDisplacementEachIteration) > 0.f)
	{
		if (DoDiscsOverlap(currentCircleOnLineCenter, lineCircleRadius, discCenter, discRadius))
			return true;

		currentCircleOnLineCenter += centerDisplacementEachIteration;
	}

	return false;
}

bool IsPointInsideDisc2D(const Vec2& point, const Vec2& discCenter, float discRadius)
{
	if (GetDistanceSquared2D(point, discCenter) < discRadius * discRadius)
		return true;
	return false;
}

const Vec2 GetNearestPointOnDisc2D(const Vec2& referencePoint, const Vec2& discCenter, float discRadius)
{
	Vec2 displacementFromCenterToPoint = referencePoint - discCenter;
	displacementFromCenterToPoint.ClampLength(discRadius);
	return discCenter + displacementFromCenterToPoint;
}

const Vec2 GetNearestPointOnAABB2D(const Vec2& referencePoint, const AABB2& box)
{
	float nearestX = referencePoint.x;
	float nearestY = referencePoint.y;

	if (nearestX < box.m_mins.x)
		nearestX = box.m_mins.x;
	else if (nearestX > box.m_maxs.x)
		nearestX = box.m_maxs.x;

	if (nearestY < box.m_mins.y)
		nearestY = box.m_mins.y;
	else if (nearestY > box.m_maxs.y)
		nearestY = box.m_maxs.y;

	return Vec2(nearestX, nearestY);
}

const Vec2 GetNearestPointOnInfiniteLine2D(const Vec2& referencePoint, const LineSegment2& infiniteLine)
{
	return GetNearestPointOnInfiniteLine2D(referencePoint, infiniteLine.m_start, infiniteLine.m_end);
}

const Vec2 GetNearestPointOnInfiniteLine2D(const Vec2& referencePoint, const Vec2& pointOnLine, const Vec2& anotherPointOnLine)
{
	Vec2 lineVector = anotherPointOnLine - pointOnLine;
	Vec2 vectorFromStartToPoint = referencePoint - pointOnLine;
	Vec2 projectedVectorOnLine = GetProjectedOnto2D(vectorFromStartToPoint, lineVector);
	return pointOnLine + projectedVectorOnLine;
}

const Vec2 GetNearestPointOnLineSegment2D(const Vec2& referencePoint, const LineSegment2& lineSegment)
{
	return GetNearestPointOnLineSegment2D(referencePoint, lineSegment.m_start, lineSegment.m_end);
}

const Vec2 GetNearestPointOnLineSegment2D(const Vec2& referencePoint, const Vec2& lineSegStart, const Vec2& lineSegEnd)
{
	Vec2 lineVector = lineSegEnd - lineSegStart;
	Vec2 vectorFromEndToPoint = referencePoint - lineSegEnd;
	if (DotProduct2D(lineVector, vectorFromEndToPoint) >= 0)
		return lineSegEnd;

	Vec2 vectorFromStartToPoint = referencePoint - lineSegStart;
	if (DotProduct2D(lineVector, vectorFromStartToPoint) <= 0)
		return lineSegStart;

	Vec2 projectedVectorOnLine = GetProjectedOnto2D(vectorFromStartToPoint, lineVector);
	return lineSegStart + projectedVectorOnLine;
}

const Vec2 GetNearestPointOnCapsule2D(const Vec2& referencePoint, const Capsule2& capsule)
{
	return GetNearestPointOnCapsule2D(referencePoint, capsule.m_bone.m_start, capsule.m_bone.m_end, capsule.m_radius);
}

const Vec2 GetNearestPointOnCapsule2D(const Vec2& referencePoint, const Vec2& boneStart, const Vec2& boneEnd, float radius)
{
	Vec2 nearestPointOnBone = GetNearestPointOnLineSegment2D(referencePoint, boneStart, boneEnd);
	Vec2 vectorFromNearestPointToReferencePoint = referencePoint - nearestPointOnBone;
	return vectorFromNearestPointToReferencePoint.GetClamped(radius) + nearestPointOnBone;
}

const Vec2 GetNearestPointOnOBB2D(const Vec2& referencePoint, const OBB2& orientedBox)
{
	Vec2 dispFromBoxCenterToPoint = referencePoint - orientedBox.m_center;
	float pointXInBoxLocalSpace = GetProjectedLength2D(dispFromBoxCenterToPoint, orientedBox.m_iBasisNormal);
	float pointYInBoxLocalSpace = GetProjectedLength2D(dispFromBoxCenterToPoint, orientedBox.m_iBasisNormal.GetRotated90Degrees());
	pointXInBoxLocalSpace = Clamp(pointXInBoxLocalSpace, -orientedBox.m_halfDimensions.x, orientedBox.m_halfDimensions.x);
	pointYInBoxLocalSpace = Clamp(pointYInBoxLocalSpace, -orientedBox.m_halfDimensions.y, orientedBox.m_halfDimensions.y);
	Vec2 nearestPoint = Vec2(pointXInBoxLocalSpace, pointYInBoxLocalSpace);
	TransformPosition2D(nearestPoint, orientedBox.m_iBasisNormal, orientedBox.m_iBasisNormal.GetRotated90Degrees(), orientedBox.m_center);
	return nearestPoint;
}

Vec3 GetNearestPointOnAABB3(const Vec3& referencePoint, const AABB3& box)
{
	Vec3 nearestPoint = referencePoint;
	nearestPoint.x = Clamp(nearestPoint.x, box.m_mins.x, box.m_maxs.x);
	nearestPoint.y = Clamp(nearestPoint.y, box.m_mins.y, box.m_maxs.y);
	nearestPoint.z = Clamp(nearestPoint.z, box.m_mins.z, box.m_maxs.z);

	return nearestPoint;
}

Vec3 GetNearestPointOnSphere3D(const Vec3& referencePoint, const Vec3& sphereCenter, float sphereRadius)
{
	Vec3 centerToPoint = referencePoint - sphereCenter;
	Vec3 centerToPointClamped = centerToPoint.GetClamped(sphereRadius);
	return sphereCenter + centerToPointClamped;
}

Vec3 GetNearestPointOnZCylinder3D(const Vec3& referencePoint, const Vec2& cylinderCenterXY, float cylinderRadius, const FloatRange& cylinderHeightZRange)
{
	Vec2 nearestPointOnCylinderDiscXY = GetNearestPointOnDisc2D(Vec2(referencePoint.x, referencePoint.y), cylinderCenterXY, cylinderRadius);
	float nearestPointOnZ = Clamp(referencePoint.z, cylinderHeightZRange.m_min, cylinderHeightZRange.m_max);
	return Vec3(nearestPointOnCylinderDiscXY.x, nearestPointOnCylinderDiscXY.y, nearestPointOnZ);
}

bool IsPointInsideOrientedSector2D(const Vec2& point, const Vec2& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius)
{
	if (GetDistanceSquared2D(point, sectorTip) > sectorRadius * sectorRadius)
		return false;

	float orientationOfDisplacementFromPointToSectorTip = (point - sectorTip).GetOrientationDegrees();
	float shortestAngleToPoint = GetShortestAngularDispDegrees(sectorForwardDegrees, orientationOfDisplacementFromPointToSectorTip);
	if (shortestAngleToPoint >  - (sectorApertureDegrees * 0.5f) &&
		shortestAngleToPoint < sectorApertureDegrees * 0.5f)
	{
		return true;
	}

	return false;
}

bool IsPointInsideDirectedSector2D(const Vec2& point, const Vec2& sectorTip, const Vec2& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius)
{
	if (GetDistanceSquared2D(point, sectorTip) > sectorRadius * sectorRadius)
		return false;

	Vec2 displacementOfPointFromSectorTip = point - sectorTip;
	float angleBetweenForwardNormalAndDisplacement = GetAngleDegreesBetweenVectors2D(sectorForwardNormal, displacementOfPointFromSectorTip);
	
	return (angleBetweenForwardNormalAndDisplacement < sectorApertureDegrees * 0.5f);
}

bool IsPointInsideAABB2D(const Vec2& point, const AABB2& box)
{
	if (point.x > box.m_mins.x &&
		point.x < box.m_maxs.x &&
		point.y > box.m_mins.y &&
		point.y < box.m_maxs.y)
	{
		return true;
	}

	return false;
}

bool IsPointInsideCapsule2D(const Vec2& point, const Capsule2& capsule)
{
	return IsPointInsideCapsule2D(point, capsule.m_bone.m_start, capsule.m_bone.m_end, capsule.m_radius);
}

bool IsPointInsideCapsule2D(const Vec2& point, const Vec2& boneStart, const Vec2& boneEnd, float radius)
{
	Vec2 nearestPointOnBone = GetNearestPointOnLineSegment2D(point, boneStart, boneEnd);
	if (GetDistanceSquared2D(nearestPointOnBone, point) < radius * radius)
		return true;

	return false;
}

bool IsPointInsideOBB2D(const Vec2& point, const OBB2& orientedBox)
{
	Vec2 dispFromBoxCenterToPoint = point - orientedBox.m_center;
	float pointXInBoxLocalSpace = GetProjectedLength2D(dispFromBoxCenterToPoint, orientedBox.m_iBasisNormal);
	float pointYInBoxLocalSpace = GetProjectedLength2D(dispFromBoxCenterToPoint, orientedBox.m_iBasisNormal.GetRotated90Degrees());
	return (pointXInBoxLocalSpace > -orientedBox.m_halfDimensions.x &&
			pointXInBoxLocalSpace < orientedBox.m_halfDimensions.x &&
			pointYInBoxLocalSpace > -orientedBox.m_halfDimensions.y &&
			pointYInBoxLocalSpace < orientedBox.m_halfDimensions.y);
}

bool IsPointInsideSphere3D(const Vec3& sphereCenter, float sphereRadius, const Vec3& point)
{
	if (GetDistanceSquared3D(sphereCenter, point) > sphereRadius * sphereRadius)
		return false;
	return true;
}

bool PushDiscOutOfPoint2D(Vec2& mobileDiscCenter, float discRadius, const Vec2& fixedPoint)
{
	if (!IsPointInsideDisc2D(fixedPoint, mobileDiscCenter, discRadius))
		return false;

	Vec2 dispFromPointToCenter = mobileDiscCenter - fixedPoint;
	Vec2 radiusVector = dispFromPointToCenter;
	radiusVector.SetLength(discRadius);
	Vec2 pushDisp = radiusVector - dispFromPointToCenter;
	mobileDiscCenter += pushDisp;
	return true;
}

bool PushDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, const Vec2& fixedDiscCenter, float fixedDiscRadius)
{
	if (!DoDiscsOverlap(mobileDiscCenter, mobileDiscRadius, fixedDiscCenter, fixedDiscRadius))
		return false;

	float distBetweenCenters = GetDistance2D(fixedDiscCenter, mobileDiscCenter);
	float distToMoveMobileDisc = fixedDiscRadius + mobileDiscRadius - distBetweenCenters;
	Vec2 dispToMove = (mobileDiscCenter - fixedDiscCenter).GetNormalized() * distToMoveMobileDisc;
	mobileDiscCenter += dispToMove;
	return true;
}

bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius)
{
	if (!DoDiscsOverlap(aCenter, aRadius, bCenter, bRadius))
		return false;

	float distBetweenCenters = GetDistance2D(aCenter, bCenter);
	float distToMoveMobileDisc = aRadius + bRadius - distBetweenCenters;
	Vec2 dispToMove = (aCenter - bCenter).GetNormalized() * distToMoveMobileDisc;
	aCenter += dispToMove * 0.5f;
	bCenter -= dispToMove * 0.5f;
	return true;
}

bool PushDiscOutOfAABB2D(Vec2& mobileDiscCenter, float discRadius, const AABB2& fixedBox)
{
	//if disc center is inside the box
	if (IsPointInsideAABB2D(mobileDiscCenter, fixedBox))
	{
		float nearestXPointOnEdge = abs(mobileDiscCenter.x - fixedBox.m_mins.x) < abs(mobileDiscCenter.x - fixedBox.m_maxs.x) ? fixedBox.m_mins.x : fixedBox.m_maxs.x;
		float nearestYPointOnEdge = abs(mobileDiscCenter.y - fixedBox.m_mins.y) < abs(mobileDiscCenter.y - fixedBox.m_maxs.y) ? fixedBox.m_mins.y : fixedBox.m_maxs.y;
		Vec2 nearestPointOnEdge = Vec2::ZERO;
		if (abs(mobileDiscCenter.x - nearestXPointOnEdge) < abs(mobileDiscCenter.y - nearestYPointOnEdge))
		{
			nearestPointOnEdge = Vec2(nearestXPointOnEdge, mobileDiscCenter.y);
		}
		else
		{
			nearestPointOnEdge = Vec2(mobileDiscCenter.x, nearestYPointOnEdge);
		}
		Vec2 pushDirection = (nearestPointOnEdge - mobileDiscCenter).GetNormalized();
		float pushDistance = discRadius + GetDistance2D(nearestPointOnEdge, mobileDiscCenter);
		Vec2 pushDisplacement = pushDirection * pushDistance;
		mobileDiscCenter += pushDisplacement;
		//DebuggerPrintf("push disp = %.2f, %.2f, center = %.2f %.2f \n", pushDisplacement.x, pushDisplacement.y, mobileDiscCenter.x, mobileDiscCenter.y);
		return true;
	}
	else
	{
		Vec2 nearestPoint = fixedBox.GetNearestPoint(mobileDiscCenter);
		if (!IsPointInsideDisc2D(mobileDiscCenter, nearestPoint, discRadius))
			return false;
		Vec2 pushDirection = mobileDiscCenter - nearestPoint;
		float overlapDepth = discRadius - pushDirection.GetLength();
		pushDirection.SetLength(overlapDepth);
		mobileDiscCenter += pushDirection;
		return true;
	}
}

bool BounceDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& fixedDiscCenter, float fixedDiscRadius, float elasticity)
{
	if (!DoDiscsOverlap(mobileDiscCenter, mobileDiscRadius, fixedDiscCenter, fixedDiscRadius))
		return false;

	PushDiscOutOfDisc2D(mobileDiscCenter, mobileDiscRadius, fixedDiscCenter, fixedDiscRadius);
	Vec2 mobileDiscNormal = (fixedDiscCenter - mobileDiscCenter).GetNormalized();
	Vec2 mobileDiscNormalVelocity = GetProjectedOnto2D(mobileDiscVelocity, mobileDiscNormal);
	Vec2 mobileDiscTangentVelocity = mobileDiscVelocity - mobileDiscNormalVelocity;
	mobileDiscVelocity = (-mobileDiscNormalVelocity * elasticity) + mobileDiscTangentVelocity;
	return true;
}

bool BounceDiscOutOfPoint2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& fixedPoint, float elasticity)
{
	return BounceDiscOutOfDisc2D(mobileDiscCenter, mobileDiscRadius, mobileDiscVelocity, fixedPoint, 0.f, elasticity);
}

bool BounceDiscOutOfEachOther(Vec2& aCenter, float aRadius, Vec2& aVelocity, Vec2& bCenter, float bRadius, Vec2&bVelocity, float elasticity)
{
	if (!DoDiscsOverlap(aCenter, aRadius, bCenter, bRadius))
		return false;

	PushDiscsOutOfEachOther2D(aCenter, aRadius, bCenter, bRadius);
	Vec2 aDiscNormal = (bCenter - aCenter).GetNormalized();

	//if the discs are diverging, then dont do anything. 
	if (DotProduct2D(bVelocity, aDiscNormal) - DotProduct2D(aVelocity, aDiscNormal) > 0.f)
		return false;

	//Vec2 aDiscNormalVelocity = GetProjectedOnto2D(aVelocity, aDiscNormal);
	Vec2 aDiscNormalVelocity = DotProduct2D(aVelocity, aDiscNormal) * aDiscNormal;
	Vec2 aDiscTangentVelocity = aVelocity - aDiscNormalVelocity;
	Vec2 bDiscNormal = -aDiscNormal;
	//Vec2 bDiscNormalVelocity = GetProjectedOnto2D(bVelocity, bDiscNormal);
	Vec2 bDiscNormalVelocity = DotProduct2D(bVelocity, bDiscNormal) * bDiscNormal;
	Vec2 bDiscTangentVelocity = bVelocity - bDiscNormalVelocity;
	aVelocity = (bDiscNormalVelocity * elasticity) + aDiscTangentVelocity;
	bVelocity = (aDiscNormalVelocity * elasticity) + bDiscTangentVelocity;
	return true;
}

bool BounceDiscOutOfCapsule2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& capsuleBoneStart, const Vec2& capsuleBoneEnd, float capsuleRadius, float elasticity)
{
	Vec2 nearestPoint = GetNearestPointOnCapsule2D(mobileDiscCenter, capsuleBoneStart, capsuleBoneEnd, capsuleRadius);
	return BounceDiscOutOfPoint2D(mobileDiscCenter, mobileDiscRadius, mobileDiscVelocity, nearestPoint, elasticity);
}

bool BounceDiscOutOfOBB2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVelocity, const OBB2& orientedBox, float elasticity)
{
	Vec2 nearestPoint = GetNearestPointOnOBB2D(mobileDiscCenter, orientedBox);
	return BounceDiscOutOfPoint2D(mobileDiscCenter, mobileDiscRadius, mobileDiscVelocity, nearestPoint, elasticity);
}

float ConvertDegreesToRadians(float degrees)
{
	return degrees * (PI / 180.f);
}

float ConvertRadiansToDegrees(float radians)
{
	return radians * (180.f / PI);
}

float CosDegrees(float degrees)
{
	return cos(degrees * PI / 180);
}

float SinDegrees(float degrees)
{
	return sin(degrees * PI / 180);
}

float Atan2Degrees(float y, float x)
{
	return ConvertRadiansToDegrees(atan2f(y, x));
}

float GetDistance2D(const Vec2& posA, const Vec2& posB)
{
	return sqrtf((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y));
}

float GetDistanceSquared2D(const Vec2& posA, const Vec2& posB)
{
	return ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y));
}

float GetDistance3D(const Vec3& posA, const Vec3& posB)
{
	return sqrtf((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z));
}

float GetDistanceSquared3D(const Vec3& posA, const Vec3& posB)
{
	return ((posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z));
}

float GetDistanceXY3D(const Vec3& posA, const Vec3& posB)
{
	return GetDistance2D(Vec2(posA.x, posA.y), Vec2(posB.x, posB.y));
}

float GetDistanceXYSquared3D(const Vec3& posA, const Vec3& posB)
{
	return GetDistanceSquared2D(Vec2(posA.x, posA.y), Vec2(posB.x, posB.y));
}

int GetTaxicabDistance2D(const IntVec2& pointA, const IntVec2& pointB)
{
	return abs(pointB.x - pointA.x) + abs(pointB.y - pointA.y);
}

float GetRandomBetween0And1()
{
	return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}

int GetIntRandomBetweenNumbers(int start, int end)
{
	return start + (rand() % (end - start));
}

bool DoDiscsOverlap(const Vec2& centerA, float radiusA, const Vec2& centerB, float radiusB)
{
	if (GetDistanceSquared2D(centerA, centerB) < ((radiusA + radiusB) * (radiusA + radiusB)))
		return true;
	return false;
}

bool DoSpheresOverlap(const Vec3& centerA, float radiusA, const Vec3& centerB, float radiusB)
{
	if (GetDistanceSquared3D(centerA, centerB) < ((radiusA + radiusB) * (radiusA + radiusB)))
		return true;
	return false;
}

bool DoAABB2sOverlap(const AABB2& boxA, const AABB2& boxB)
{
	if (boxB.m_mins.x >= boxA.m_maxs.x ||
		boxA.m_mins.x >= boxB.m_maxs.x ||
		boxA.m_mins.y >= boxB.m_maxs.y ||
		boxB.m_mins.y >= boxA.m_maxs.y)
	{
		return false;
	}

	return true;
}

bool DoAABB3sOverlap(const AABB3& boxA, const AABB3& boxB)
{
	if (boxB.m_mins.x >= boxA.m_maxs.x ||
		boxA.m_mins.x >= boxB.m_maxs.x ||
		boxA.m_mins.y >= boxB.m_maxs.y ||
		boxB.m_mins.y >= boxA.m_maxs.y ||
		boxA.m_mins.z >= boxB.m_maxs.z ||
		boxB.m_mins.z >= boxA.m_maxs.z)
	{
		return false;
	}

	return true;
}

bool DoesDiscOverlapAABB2(const Vec2& discCenter, float discRadius, const AABB2& box)
{
	Vec2 nearestPointOnABB2 = box.GetNearestPoint(discCenter);
	return IsPointInsideDisc2D(nearestPointOnABB2, discCenter, discRadius);
}

bool DoesSphereOverlapAABB3(const Vec3& sphereCenter, float sphereRadius, const AABB3& box)
{
	Vec3 nearestPointOnAABB3 = GetNearestPointOnAABB3(sphereCenter, box);
	return IsPointInsideSphere3D(sphereCenter, sphereRadius, nearestPointOnAABB3);
}

bool DoesSphereOverlapZCylinder(const Vec3& sphereCenter, float sphereRadius, const Vec2& cylinderACenterXY, float cylinderARadius, FloatRange cylinderAHeightRange)
{
	Vec3 nearestPoint = GetNearestPointOnZCylinder3D(sphereCenter, cylinderACenterXY, cylinderARadius, cylinderAHeightRange);
	return IsPointInsideSphere3D(sphereCenter, sphereRadius, nearestPoint);
}

bool DoZCylindersOverlap3D(const Vec2& cylinderACenterXY, float cylinderARadius, FloatRange cylinderAHeightRange, const Vec2& cylinderBCenterXY, float cylinderBRadius, FloatRange cylinderBHeightRange)
{
	return (DoDiscsOverlap(cylinderACenterXY, cylinderARadius, cylinderBCenterXY, cylinderBRadius) && cylinderAHeightRange.IsOverlappingWith(cylinderBHeightRange));
}

bool DoesZCylinderOverlapAABB3(const Vec2& cylinderCenterXY, float cylinderRadius, FloatRange cylinderHeightRange, const AABB3& box)
{
	return (DoesDiscOverlapAABB2(cylinderCenterXY, cylinderRadius, AABB2(Vec2(box.m_mins.x, box.m_mins.y), Vec2(box.m_maxs.x, box.m_maxs.y))) &&
		cylinderHeightRange.IsOverlappingWith(FloatRange(box.m_mins.z, box.m_maxs.z)));
}

