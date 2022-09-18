#include "Engine/Math/Vec4.hpp"
#include <cmath>

Vec4::Vec4(float initialX, float initialY, float initialZ, float initialW)
	:x(initialX), y(initialY), z(initialZ), w(initialW)
{
}

const Vec4 Vec4::operator-(const Vec4& vectorToSubtract) const
{
	return Vec4(x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z, w - vectorToSubtract.w);
}

const Vec4 Vec4::operator*(const Vec4& vectorToMultiply) const
{
	return Vec4(x * vectorToMultiply.x, y * vectorToMultiply.y, z * vectorToMultiply.z, w * vectorToMultiply.w);
}

const Vec4 Vec4::operator*(float uniformScale) const
{
	return Vec4(x * uniformScale, y * uniformScale, z * uniformScale, w * uniformScale);
}

const Vec4 Vec4::operator/(float inverseScale) const
{
	return Vec4(x / inverseScale, y / inverseScale, z / inverseScale, w / inverseScale);
}

const void Vec4::operator*=(float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
	w *= uniformScale;
}

float Vec4::GetLength() const
{
	return sqrtf(x * x + y * y + z * z + w * w);
}

void Vec4::Normalize()
{
	float length = GetLength();
	if (length == 0.f)
		return;
	*this = (*this) / GetLength();
}
