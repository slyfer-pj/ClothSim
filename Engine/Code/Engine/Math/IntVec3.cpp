#include "Engine/Math/IntVec3.hpp"
#include "Engine/Core/StringUtils.hpp"

const IntVec3 IntVec3::ZERO = IntVec3(0, 0, 0);
const IntVec3 IntVec3::ONE = IntVec3(1, 1, 1);

IntVec3::IntVec3(const IntVec3& copyFrom)
	:x(copyFrom.x), y(copyFrom.y), z(copyFrom.z)
{
}

IntVec3::IntVec3(int initialX, int initialY, int initialZ)
	:x(initialX), y(initialY), z(initialZ)
{
}

IntVec3 IntVec3::operator+(const IntVec3& vecToAdd) const
{
	return IntVec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}

void IntVec3::SetFromText(const char* text)
{
	Strings subStrings = SplitStringOnDelimiter(text, ',');
	x = atoi(subStrings[0].c_str());
	y = atoi(subStrings[1].c_str());
	z = atoi(subStrings[2].c_str());
}
