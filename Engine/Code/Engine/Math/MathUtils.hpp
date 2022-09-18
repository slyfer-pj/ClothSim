#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/OBB2.hpp"

struct AABB3;
struct FloatRange;
struct RaycastHit2D
{
	bool m_didImpact = false;
	float m_impactDistance = 0.f;
	Vec2 m_impactPosition = Vec2::ZERO;
	Vec2 m_impactNormal = Vec2::ZERO;
};

struct RaycastHit3D
{
	bool m_didImpact = false;
	float m_impactDistance = 0.f;
	Vec3 m_impactPosition = Vec3::ZERO;
	Vec3 m_impactNormal = Vec3::ZERO;
};


float ConvertDegreesToRadians(float degrees);
float ConvertRadiansToDegrees(float radians);
float CosDegrees(float degrees);
float SinDegrees(float degrees);
float Atan2Degrees(float y, float x);

float GetDistance2D(const Vec2& posA, const Vec2& posB);
float GetDistanceSquared2D(const Vec2& posA, const Vec2& posB);
float GetDistance3D(const Vec3& posA, const Vec3& posB);
float GetDistanceSquared3D(const Vec3& posA, const Vec3& posB);
float GetDistanceXY3D(const Vec3& posA, const Vec3& posB);
float GetDistanceXYSquared3D(const Vec3& posA, const Vec3& posB);
int GetTaxicabDistance2D(const IntVec2& pointA, const IntVec2& pointB);
float GetProjectedLength2D(const Vec2& vectorToProject, const Vec2& vectorToProjectOnto);
const Vec2 GetProjectedOnto2D(const Vec2& vectorToProject, const Vec2& vectorToProjectOnto);
float GetAngleDegreesBetweenVectors2D(const Vec2& a, const Vec2& b);
float GetAngleDegreesBetweenVectors3D(const Vec3& a, const Vec3& b);

float DotProduct2D(const Vec2& a, const Vec2& b);
float DotProduct3D(const Vec3& a, const Vec3& b);
float DotProduct4D(const Vec4& a, const Vec4& b);
float CrossProduct2D(const Vec2& a, const Vec2& b);
Vec3 CrossProduct3D(const Vec3& a, const Vec3& b);

bool DoDiscsOverlap(const Vec2& centerA,  float radiusA, const Vec2& centerB, float radiusB);
bool DoSpheresOverlap(const Vec3& centerA,  float radiusA, const Vec3& centerB, float radiusB);
bool DoAABB2sOverlap(const AABB2& boxA, const AABB2& boxB);
bool DoAABB3sOverlap(const AABB3& boxA, const AABB3& boxB);
bool DoesDiscOverlapAABB2(const Vec2& discCenter, float discRadius, const AABB2& box);
bool DoesSphereOverlapAABB3(const Vec3& sphereCenter, float sphereRadius, const AABB3& box);
bool DoesSphereOverlapZCylinder(const Vec3& sphereCenter, float sphereRadius, const Vec2& cylinderACenterXY, float cylinderARadius, FloatRange cylinderAHeightRange);
bool DoZCylindersOverlap3D(const Vec2& cylinderACenterXY, float cylinderARadius, FloatRange cylinderAHeightRange, const Vec2& cylinderBCenterXY, float cylinderBRadius, FloatRange cylinderBHeightRange);
bool DoesZCylinderOverlapAABB3(const Vec2& cylinderCenterXY, float cylinderRadius, FloatRange cylinderHeightRange, const AABB3& box);
bool CheckIfDiscOverlapsWithLine(const Vec2& discCenter, float discRadius, const Vec2& lineStart, const Vec2& lineEnd);
bool IsPointInsideDisc2D(const Vec2& point, const Vec2& discCenter,float discRadius);
bool IsPointInsideOrientedSector2D(const Vec2& point, const Vec2& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius);
bool IsPointInsideDirectedSector2D(const Vec2& point, const Vec2& sectorTip, const Vec2& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius);
bool IsPointInsideAABB2D(const Vec2& point, const AABB2& box);
bool IsPointInsideCapsule2D(const Vec2& point, const Capsule2& capsule);
bool IsPointInsideCapsule2D(const Vec2& point, const Vec2& boneStart, const Vec2& boneEnd, float radius);
bool IsPointInsideOBB2D(const Vec2& point, const OBB2& orientedBox);
bool IsPointInsideSphere3D(const Vec3& sphereCenter, float sphereRadius, const Vec3& point);
bool PushDiscOutOfPoint2D(Vec2& mobileDiscCenter, float discRadius, const Vec2& fixedPoint);
bool PushDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, const Vec2& fixedDiscCenter, float fixedDiscRadius);
bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius);
bool PushDiscOutOfAABB2D(Vec2& mobileDiscCenter, float discRadius, const AABB2& fixedBox);
bool BounceDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& fixedDiscCenter, float fixedDiscRadius, float elasticity);
bool BounceDiscOutOfPoint2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& fixedPoint, float elasticity);
bool BounceDiscOutOfEachOther(Vec2& aCenter, float aRadius, Vec2& aVelocity, Vec2& bCenter, float bRadius, Vec2& bVelocity, float elasticity);
bool BounceDiscOutOfCapsule2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVelocity, const Vec2& capsuleBoneStart, const Vec2& capsuleBoneEnd, float capsuleRadius, float elasticity);
bool BounceDiscOutOfOBB2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2& mobileDiscVelocity, const OBB2& orientedBox, float elasticity);
const Vec2 GetNearestPointOnDisc2D(const Vec2& referencePoint, const Vec2& discCenter, float discRadius);
const Vec2 GetNearestPointOnAABB2D(const Vec2& referencePoint, const AABB2& box);
const Vec2 GetNearestPointOnInfiniteLine2D(const Vec2& referencePoint, const LineSegment2& infiniteLine);
const Vec2 GetNearestPointOnInfiniteLine2D(const Vec2& referencePoint, const Vec2& pointOnLine, const Vec2& anotherPointOnLine);
const Vec2 GetNearestPointOnLineSegment2D(const Vec2& referencePoint, const LineSegment2& lineSegment);
const Vec2 GetNearestPointOnLineSegment2D(const Vec2& referencePoint, const Vec2& lineSegStart, const Vec2& lineSegEnd);
const Vec2 GetNearestPointOnCapsule2D(const Vec2& referencePoint, const Capsule2& capsule);
const Vec2 GetNearestPointOnCapsule2D(const Vec2& referencePoint, const Vec2& boneStart, const Vec2& boneEnd, float radius);
const Vec2 GetNearestPointOnOBB2D(const Vec2& referencePoint, const OBB2& orientedBox);
Vec3 GetNearestPointOnAABB3(const Vec3& referencePoint, const AABB3& box);
Vec3 GetNearestPointOnSphere3D(const Vec3& referencePoint, const Vec3& sphereCenter, float sphereRadius);
Vec3 GetNearestPointOnZCylinder3D(const Vec3& referencePoint, const Vec2& cylinderCenterXY, float cylinderRadius, const FloatRange& cylinderHeightZRange);

void TransformPosition2D(Vec2& positionToTransform, float uniformScaleXY, float rotationDegreeAboutZ, const Vec2& translationXY);
void TransformPosition2D(Vec2& positionToTransform, const Vec2& iBasis, const Vec2& jBasis, const Vec2& translationXY);
void TransformPositionXY3D(Vec3& positionToTransform, float uniformScaleXY, float rotationDegreeAboutZ, const Vec2& translationXY);
void TransformPositionXY3D(Vec3& positionToTransform, const Vec2& iBasis, const Vec2& jBasis, const Vec2& translationXY);

float GetFractionWithin(float inputValue, float inputRangeStart, float inputRangeEnd);
//float Interpolate(float start, float end, float fraction);
template <class T>
T Interpolate(T start, T end, float fraction)
{
	return start + (T((end - start) * fraction));
}
float RangeMap(float inputValue, float inputRangeStart, float inputRangeEnd, float outputRangeStart, float outputRangeEnd);
float RangeMapClamped(float inputValue, float inputRangeStart, float inputRangeEnd, float outputRangeStart, float outputRangeEnd);
float ClampZeroToOne(float value);

template <class T>
T Clamp(T value, T min, T max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;

	return value;
}

int RoundDownToInt(float value);

float GetShortestAngularDispDegrees(float fromDegrees, float toDegrees);
float GetTurnedTowardDegrees(float fromDegrees, float toDegrees, float maxDeltaDegrees);

RaycastHit2D RaycastVsDisc(const Vec2& rayOrigin, const Vec2& rayForwardNormal, float rayMaxLength, const Vec2& discCenter, float discRadius);
RaycastHit2D RaycastVsAABB2(const Vec2& rayOrigin, const Vec2& rayForwardNormal, float rayMaxLength, AABB2 bounds);
RaycastHit3D RaycastVsSphere3D(const Vec3& rayOrigin, const Vec3& rayForwardNormal, float rayMaxLength, const Vec3& sphereCenter, float sphereRadius);
RaycastHit3D RaycastVsAABB3(const Vec3& rayOrigin, const Vec3& rayForwardNormal, float rayMaxLength, AABB3 bounds);
RaycastHit3D RaycastVsZCylinder(const Vec3& rayOrigin, const Vec3& rayForwardNormal, float rayMaxLength, const Vec2& zCylinderXYCenter, float zCylinderRadius, FloatRange zCylinderHeightRange);
RaycastHit2D RaycastVsLineSegment(const Vec2& rayOrigin, const Vec2& rayForwardNormal, float rayMaxLength, const Vec2& lineSegStart, const Vec2& lineSegEnd);
RaycastHit2D RaycastVsOBB2(const Vec2& rayOrigin, const Vec2& rayForwardNormal, float rayMaxLength, const OBB2& orientedBox);

float NormalizeByte(unsigned char byteValue);
unsigned char DenormalizeByte(float zeroToOne);

float SmoothStart2(float paramtericZeroToOne);
float SmoothStart3(float paramtericZeroToOne);
float SmoothStart4(float paramtericZeroToOne);
float SmoothStart5(float paramtericZeroToOne);
float SmoothStart6(float paramtericZeroToOne);
float SmoothStop2(float paramtericZeroToOne);
float SmoothStop3(float paramtericZeroToOne);
float SmoothStop4(float paramtericZeroToOne);
float SmoothStop5(float paramtericZeroToOne);
float SmoothStop6(float paramtericZeroToOne);
float SmoothStep3(float paramtericZeroToOne);
float SmoothStep5(float paramtericZeroToOne);
float Hesitate3(float paramtericZeroToOne);
float Hesitate5(float paramtericZeroToOne);

float ComputeCubicBezier1D(float A, float B, float C, float D, float t);
float ComputeQuinticBezier1D(float A, float B, float C, float D, float E, float F, float t);

