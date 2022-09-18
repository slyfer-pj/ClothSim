#pragma once

struct IntVec3;
struct Vec2;

struct Vec3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

public:
	Vec3() = default;
	explicit Vec3(float x, float y, float z);
	explicit Vec3(const IntVec3& fromIntVector);
	explicit Vec3(const Vec2& fromVec2);

	float GetLength() const;
	float GetLengthXY() const;
	float GetLengthSquared() const;
	float GetLengthXYSquared() const;
	float GetAngleAboutZRadians() const;
	float GetAngleAboutZDegrees() const;
	float GetAngleAboutYRadians() const;
	float GetAngleAboutYDegrees() const;
	const Vec3 GetRotatedAboutZRadians(float deltaRadians) const;
	const Vec3 GetRotatedAboutZDegrees(float deltaDegrees) const;
	const Vec3 GetClamped(float maxLength) const;
	const Vec3 GetNormalized() const;

	void SetFromText(const char* text);

	bool operator==(const Vec3& compare) const;
	bool operator!=(const Vec3& compare) const;
	const Vec3 operator+(const Vec3& vecToAdd) const;
	const Vec3 operator-(const Vec3& vecToSubtract) const;
	const Vec3 operator-() const;
	const Vec3 operator*(float uniformScale) const;
	const Vec3 operator*(const Vec3& vectorToMultiply) const;
	const Vec3 operator/(float inverseScale) const;

	void operator+=(const Vec3& vecToAdd);
	void operator-=(const Vec3& vecToSubtract);
	void operator*=(float uniformScale);
	void operator/=(float inverseScale);
	void operator=(const Vec3& copyFrom);

	friend const Vec3 operator*(float uniformScale, const Vec3& vecToScale);
	
	static const Vec3 ZERO;
	static const Vec3 ONE;
};