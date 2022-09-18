#pragma once

struct IntRange
{
public:
	int m_min = 0;
	int m_max = 0;

	static const IntRange ZERO;
	static const IntRange ONE;
	static const IntRange ZERO_TO_ONE;

public:
	IntRange() {};
	IntRange(int min, int max);
	void operator=(const IntRange& copyFrom);
	bool operator==(const IntRange& compare) const;
	bool operator!=(const IntRange& compare) const;
	bool IsOnRange(int value) const;
	bool IsOverlappingWith(const IntRange& overlapRange) const;
};