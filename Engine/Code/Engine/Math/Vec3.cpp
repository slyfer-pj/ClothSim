#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/IntVec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include <cmath>

const Vec3 Vec3::ZERO = Vec3();
const Vec3 Vec3::ONE = Vec3(1.f, 1.f, 1.f);

Vec3::Vec3(float x, float y, float z)
	:x(x), y(y), z(z)
{
}

Vec3::Vec3(const IntVec3& fromIntVector)
	:x(float(fromIntVector.x)), y(float(fromIntVector.y)), z(float(fromIntVector.z))
{
}

Vec3::Vec3(const Vec2& fromVec2)
	:x(fromVec2.x), y(fromVec2.y), z(0.f)
{

}

float Vec3::GetLength() const
{
	return sqrtf(x * x + y * y + z * z);
}

float Vec3::GetLengthXY() const
{
	return sqrtf(x * x + y * y);
}

float Vec3::GetLengthSquared() const
{
	return (x * x + y * y + z * z);
}

float Vec3::GetLengthXYSquared() const
{
	return (x * x + y * y);
}

float Vec3::GetAngleAboutZRadians() const
{
	return ConvertDegreesToRadians(Atan2Degrees(y, x));
}

float Vec3::GetAngleAboutZDegrees() const
{
	return Atan2Degrees(y, x);
}

float Vec3::GetAngleAboutYRadians() const
{
	float Vxy = Vec2(x, y).GetLength();
	return atan2f(-z, Vxy);
	//return asinf(-z);
}

float Vec3::GetAngleAboutYDegrees() const
{
	return ConvertRadiansToDegrees(GetAngleAboutYRadians());
}

const Vec3 Vec3::GetRotatedAboutZRadians(float deltaRadians) const
{
	float thetaRadians = GetAngleAboutZRadians();
	thetaRadians += deltaRadians;
	return Vec3(GetLengthXY() * CosDegrees(ConvertRadiansToDegrees(thetaRadians)), GetLengthXY() * SinDegrees(ConvertRadiansToDegrees(thetaRadians)), z);
}

const Vec3 Vec3::GetRotatedAboutZDegrees(float deltaDegrees) const
{
	float thetaDegrees = GetAngleAboutZDegrees();
	thetaDegrees += deltaDegrees;
	return Vec3(GetLengthXY() * CosDegrees(thetaDegrees), GetLengthXY() * SinDegrees(thetaDegrees), z);
}

const Vec3 Vec3::GetClamped(float maxLength) const
{
	if (GetLength() > maxLength)
		return GetNormalized() * maxLength;
	return *this;
}

const Vec3 Vec3::GetNormalized() const
{
	float length = GetLength();
	if (length == 0.f)
		return *this;
	return (*this) / length;
}

void Vec3::SetFromText(const char* text)
{
	Strings subStrings = SplitStringOnDelimiter(text, ',');
	x = (float)atof(subStrings[0].c_str());
	y = (float)atof(subStrings[1].c_str());
	z = (float)atof(subStrings[2].c_str());
}

bool Vec3::operator!=(const Vec3& compare) const
{
	if (x != compare.x || y != compare.y || z != compare.z)
		return true;
	return false;
}

bool Vec3::operator==(const Vec3& compare) const
{
	return (x == compare.x && y == compare.y && z == compare.z);
}

const Vec3 Vec3::operator+(const Vec3& vecToAdd) const
{
	return Vec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}

const Vec3 Vec3::operator-(const Vec3& vecToSubtract) const
{
	return Vec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}

const Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

const Vec3 Vec3::operator*(float uniformScale) const
{
	return Vec3(x * uniformScale, y * uniformScale, z * uniformScale);
}

const Vec3 Vec3::operator*(const Vec3& vectorToMultiply) const
{
	return Vec3(x * vectorToMultiply.x, y * vectorToMultiply.y, z * vectorToMultiply.z);
}

const Vec3 Vec3::operator/(float inverseScale) const
{
	return Vec3(x / inverseScale, y / inverseScale, z / inverseScale);
}

void Vec3::operator/=(float inverseScale)
{
	x /= inverseScale;
	y /= inverseScale;
	z /= inverseScale;
}

void Vec3::operator=(const Vec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}



void Vec3::operator*=(float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}

void Vec3::operator-=(const Vec3& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}

void Vec3::operator+=(const Vec3& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

const Vec3 operator*(float unifromScale, const Vec3& vecToScale)
{
	return Vec3(unifromScale * vecToScale.x, unifromScale * vecToScale.y, unifromScale * vecToScale.z);
}


