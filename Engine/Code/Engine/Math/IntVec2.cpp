#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <cmath>

const IntVec2 IntVec2::ZERO = IntVec2(0, 0);
const IntVec2 IntVec2::ONE = IntVec2(1, 1);

IntVec2::IntVec2(const IntVec2& copyFrom)
	: x(copyFrom.x), y(copyFrom.y)
{

}

IntVec2::IntVec2(int initialX, int initialY)
	: x(initialX), y(initialY)
{

}

float IntVec2::GetLength() const
{
	return sqrtf((float)((x * x) + (y * y)));
}

int IntVec2::GetLengthSquared() const
{
	return (x * x) + (y * y);
}

int IntVec2::GetTaxicabLength() const
{
	return abs(x) + abs(y);
}

const IntVec2 IntVec2::GetRotated90Degrees() const
{
	return IntVec2(-y, x);
}

const IntVec2 IntVec2::GetRotatedMinus90Degrees() const
{
	return IntVec2(y, -x);
}

float IntVec2::GetOrientationDegrees() const
{
	return Atan2Degrees((float)y, (float)x);
}

float IntVec2::GetOrientationRadians() const
{
	return ConvertDegreesToRadians(GetOrientationDegrees());
}

void IntVec2::Rotate90Degrees()
{
	int temp = x;
	x = -y;
	y = temp;
}

void IntVec2::RotateMinus90Degrees()
{
	int temp = x;
	x = y;
	y = -temp;
}

void IntVec2::SetFromText(const char* text)
{
	Strings subStrings = SplitStringOnDelimiter(text, ',');
	x = atoi(subStrings[0].c_str());
	y = atoi(subStrings[1].c_str());
}

const IntVec2 IntVec2::operator+(const IntVec2& intVecToAdd) const
{
	return IntVec2(x + intVecToAdd.x, y + intVecToAdd.y);
}

const IntVec2 IntVec2::operator-(const IntVec2& intVecToSubtract) const
{
	return IntVec2(x - intVecToSubtract.x, y - intVecToSubtract.y);
}

const bool IntVec2::operator!=(const IntVec2& intVecToCompare) const
{
	return (x != intVecToCompare.x || y != intVecToCompare.y);
}

const bool IntVec2::operator==(const IntVec2& intVecToCompare) const
{
	return (x == intVecToCompare.x && y == intVecToCompare.y);
}

