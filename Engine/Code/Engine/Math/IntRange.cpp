#include "Engine/Math/IntRange.hpp"

IntRange::IntRange(int min, int max)
	:m_min(min), m_max(max)
{
}

bool IntRange::IsOnRange(int value) const
{
	return (value >= m_min && value <= m_max);
}

bool IntRange::IsOverlappingWith(const IntRange& overlapRange) const
{
	if (m_min <= overlapRange.m_min)
		return m_max >= overlapRange.m_min;
	else
		return overlapRange.m_max >= m_min;
}

bool IntRange::operator!=(const IntRange& compare) const
{
	return (m_min != compare.m_min || m_max != compare.m_max);
}

bool IntRange::operator==(const IntRange& compare) const
{
	return (m_min == compare.m_min && m_max == compare.m_max);
}

void IntRange::operator=(const IntRange& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}
