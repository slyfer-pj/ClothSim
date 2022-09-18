#pragma once

struct Vec4
{
	float x;
	float y;
	float z;
	float w;

	Vec4() = default;
	explicit Vec4(float initialX, float initialY, float initialZ, float initialW);

	const Vec4 operator-(const Vec4& vectorToSubtract) const;
	const Vec4 operator*(float uniformScale) const;
	const Vec4 operator*(const Vec4& vectorToMultiply) const;
	const Vec4 operator/(float inverseScale) const;
	const void operator*=(float uniformScale);

	float GetLength() const;
	void Normalize();
};