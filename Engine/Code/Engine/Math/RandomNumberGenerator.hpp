#pragma once
#include "Engine/Math/Vec3.hpp"

class RandomNumberGenerator
{
public:
	int GetRandomIntLessThan(int maxNotInclusive) const;
	int GetRandomIntInRange(int minInclusive, int maxInclusive) const;
	float GetRandomFloatZeroToOne() const;
	float GetRandomFloatInRange(float minInclusive, float maxInclusive) const;
	Vec3 GetRandomDirectionInCone(const Vec3& forward, float angle) const;

};