#pragma once
#include <string>

struct FloatRange
{
public:
	float m_min = 0.f;
	float m_max = 0.f;

	static const FloatRange ZERO;
	static const FloatRange ONE;
	static const FloatRange ZERO_TO_ONE;

public:
	FloatRange() {};
	FloatRange(float min, float max);
	void operator=(const FloatRange& copyFrom);
	bool operator==(const FloatRange& compare) const;
	bool operator!=(const FloatRange& compare) const;
	bool IsOnRange(float value) const;
	bool IsOverlappingWith(const FloatRange& overlapRange) const;
	void SetFromText(const char* text);
	std::string ToXMLString() const;
};