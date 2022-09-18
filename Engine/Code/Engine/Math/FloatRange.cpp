#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/StringUtils.hpp"

const FloatRange FloatRange::ZERO = FloatRange(0.f, 0.f);
const FloatRange FloatRange::ONE = FloatRange(1.f, 1.f);
const FloatRange FloatRange::ZERO_TO_ONE = FloatRange(0.f, 1.f);

FloatRange::FloatRange(float min, float max)
	:m_min(min), m_max(max)
{
}

bool FloatRange::IsOnRange(float value) const
{
	return (value >= m_min && value <= m_max);
}

bool FloatRange::IsOverlappingWith(const FloatRange& overlapRange) const
{
	if (m_min <= overlapRange.m_min)
		return m_max >= overlapRange.m_min;
	else
		return overlapRange.m_max >= m_min;
}

void FloatRange::SetFromText(const char* text)
{
	Strings subStrings = SplitStringOnDelimiter(text, '~');
	m_min = (float)atof(subStrings[0].c_str());
	m_max = (float)atof(subStrings[1].c_str());
}

std::string FloatRange::ToXMLString() const
{
	//string formatted such that xml parsing functions can decipher the values
	return Stringf("%.2f~%.2f", m_min, m_max);
}

bool FloatRange::operator!=(const FloatRange& compare) const
{
	return (m_min != compare.m_min || m_max != compare.m_max);
}

bool FloatRange::operator==(const FloatRange& compare) const
{
	return (m_min == compare.m_min && m_max == compare.m_max);
}

void FloatRange::operator=(const FloatRange& copyFrom)
{
	m_min = copyFrom.m_min;
	m_max = copyFrom.m_max;
}

