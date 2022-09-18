#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <cmath>
//#include "Engine/Core/EngineCommon.hpp"

const Vec2 Vec2::ZERO = Vec2(0.f, 0.f);
const Vec2 Vec2::ONE = Vec2(1.f, 1.f);

//-----------------------------------------------------------------------------------------------
Vec2::Vec2( const Vec2& copy )
	: x( copy.x )
	, y( copy.y )
{
}


//-----------------------------------------------------------------------------------------------
Vec2::Vec2( float initialX, float initialY )
	: x( initialX )
	, y( initialY )
{
}

Vec2::Vec2(IntVec2 intVec)
	:x((float)intVec.x), y((float)intVec.y)
{

}

Vec2 const Vec2::MakeFromPolarRadians(float orientationRadians, float length)
{
	Vec2 returnVector;
	returnVector.x = length * CosDegrees(ConvertRadiansToDegrees(orientationRadians));
	returnVector.y = length * SinDegrees(ConvertRadiansToDegrees(orientationRadians));
	return returnVector;
}


Vec2 const Vec2::MakeFromPolarDegrees(float orientationDegrees, float length)
{
	Vec2 returnVector;
	returnVector.x = length * CosDegrees(orientationDegrees);
	returnVector.y = length * SinDegrees(orientationDegrees);
	return returnVector;
}

float Vec2::GetLength() const
{
	return sqrtf(x * x + y * y);
}

float Vec2::GetLengthSquared() const
{
	return (x * x + y * y);
}

float Vec2::GetOrientationRadians() const
{
	return ConvertDegreesToRadians(Atan2Degrees(y, x));
}

float Vec2::GetOrientationDegrees() const
{
	return Atan2Degrees(y, x);
}

const Vec2 Vec2::GetRotated90Degrees() const
{
	return Vec2(-y, x);
}

const Vec2 Vec2::GetRotatedMinus90Degrees() const
{
	return Vec2(y, -x);
}

const Vec2 Vec2::GetRotatedRadians(float deltaRadians) const
{
	float thetaRadians = ConvertDegreesToRadians(Atan2Degrees(y, x));
	thetaRadians += deltaRadians;
	float magnitude = GetLength();
	float rotX = magnitude * CosDegrees(ConvertRadiansToDegrees(thetaRadians));
	float rotY = magnitude * SinDegrees(ConvertRadiansToDegrees(thetaRadians));
	return Vec2(rotX, rotY);
}

const Vec2 Vec2::GetRotatedDegrees(float deltaDegrees) const
{
	float thetaDegrees = Atan2Degrees(y, x);
	thetaDegrees += deltaDegrees;
	float magnitude = GetLength();
	float rotX = magnitude * CosDegrees(thetaDegrees);
	float rotY = magnitude * SinDegrees(thetaDegrees);
	return Vec2(rotX, rotY);
}

const Vec2 Vec2::GetClamped(float maxLength) const
{
	if (GetLength() > maxLength)
	{
		float thetaDegrees = Atan2Degrees(y, x);
		return Vec2(maxLength * CosDegrees(thetaDegrees), maxLength * SinDegrees(thetaDegrees));
	}
	return *this;
}

const Vec2 Vec2::GetNormalized() const
{
	float length = GetLength();
	if (length == 0.f)
		return *this;
	return *this / length;
}

const Vec2 Vec2::GetReflected(const Vec2& impactSurfaceNormal) const
{
	Vec2 incomingRayProjectedOntoNormal = DotProduct2D(*this, impactSurfaceNormal) * impactSurfaceNormal;
	Vec2 incomingTangentComponent = *this - incomingRayProjectedOntoNormal;
	return incomingTangentComponent + (-incomingRayProjectedOntoNormal);
}

void Vec2::SetOrientationRadians(float newOrientationRadians)
{
	float currentLength = GetLength();
	x = currentLength * CosDegrees(ConvertRadiansToDegrees(newOrientationRadians));
	y = currentLength * SinDegrees(ConvertRadiansToDegrees(newOrientationRadians));
}

void Vec2::SetOrientationDegrees(float newOrientationDegrees)
{
	float currentLength = GetLength();
	x = currentLength * CosDegrees(newOrientationDegrees);
	y = currentLength * SinDegrees(newOrientationDegrees);
}

void Vec2::SetPolarRadians(float newOrientationRadians, float newLength)
{
	x = newLength * CosDegrees(ConvertRadiansToDegrees(newOrientationRadians));
	y = newLength * SinDegrees(ConvertRadiansToDegrees(newOrientationRadians));
}

void Vec2::SetPolarDegrees(float newOrientationDegrees, float newLength)
{
	x = newLength * CosDegrees(newOrientationDegrees);
	y = newLength * SinDegrees(newOrientationDegrees);
}

void Vec2::Rotate90Degrees()
{
	float temp = x;
	x = -y;
	y = temp;
}

void Vec2::RotateMinus90Degrees()
{
	float temp = x;
	x = y;
	y = -temp;
}

void Vec2::RotateRadians(float deltaRadians)
{
	float thetaRadians = ConvertDegreesToRadians(Atan2Degrees(y, x));
	thetaRadians += deltaRadians;
	float magnitude = GetLength();
	x = magnitude * CosDegrees(ConvertRadiansToDegrees(thetaRadians));
	y = magnitude * SinDegrees(ConvertRadiansToDegrees(thetaRadians));
}

void Vec2::RotateDegrees(float deltaDegrees)
{
	float thetaDegrees = Atan2Degrees(y, x);
	thetaDegrees += deltaDegrees;
	float magnitude = GetLength();
	x = magnitude * CosDegrees(thetaDegrees);
	y = magnitude * SinDegrees(thetaDegrees);
}

void Vec2::SetLength(float newLength)
{
	float thetaDegrees = Atan2Degrees(y, x);
	x = newLength * CosDegrees(thetaDegrees);
	y = newLength * SinDegrees(thetaDegrees);
}

void Vec2::ClampLength(float maxLength)
{
	if (GetLength() > maxLength)
		SetLength(maxLength);
}

void Vec2::Normalize()
{
	float length = GetLength();
	if (length == 0.f)
		return;
	*this = (*this) / GetLength();
}

float Vec2::NormalizeAndGetPreviousLength()
{
	float currentLength = GetLength();
	Normalize();
	return currentLength;
}

void Vec2::Reflect(const Vec2& impactSurfaceNormal)
{
	Vec2 incomingRayProjectedOntoNormal = DotProduct2D(*this, impactSurfaceNormal) * impactSurfaceNormal;
	Vec2 incomingTangentComponent = *this - incomingRayProjectedOntoNormal;
	*this = incomingTangentComponent + (-incomingRayProjectedOntoNormal);
}

void Vec2::SetFromText(const char* text)
{
	Strings subStrings = SplitStringOnDelimiter(text, ',');
	x = (float)atof(subStrings[0].c_str());
	y = (float)atof(subStrings[1].c_str());
}

//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator + ( const Vec2& vecToAdd ) const
{
	return Vec2( x + vecToAdd.x, y + vecToAdd.y );
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-( const Vec2& vecToSubtract ) const
{
	return Vec2(x - vecToSubtract.x, y - vecToSubtract.y);
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-() const
{
	return Vec2( -x, -y );
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*( float uniformScale ) const
{
	return Vec2( x * uniformScale, y * uniformScale );
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*( const Vec2& vecToMultiply ) const
{
	return Vec2( x * vecToMultiply.x, y * vecToMultiply.y );
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator/( float inverseScale ) const
{
	return Vec2( x / inverseScale, y / inverseScale );
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator+=( const Vec2& vecToAdd )
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator-=( const Vec2& vecToSubtract )
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator*=( const float uniformScale )
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator/=( const float uniformDivisor )
{
	x /= uniformDivisor;
	y /= uniformDivisor;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator=( const Vec2& copyFrom )
{
	x = copyFrom.x;
	y = copyFrom.y;
}


//-----------------------------------------------------------------------------------------------
const Vec2 operator*( float uniformScale, const Vec2& vecToScale )
{
	return Vec2( vecToScale * uniformScale );
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator==( const Vec2& compare ) const
{
	if (x == compare.x && y == compare.y)
		return true;
	return false;
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator!=( const Vec2& compare ) const
{
	if (x != compare.x || y != compare.y)
		return true;
	return false;
}



