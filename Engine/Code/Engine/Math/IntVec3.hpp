#pragma once

struct IntVec3
{
public:
	int x = 0;
	int y = 0;
	int z = 0;

	static const IntVec3 ZERO;
	static const IntVec3 ONE;

public:
	IntVec3() {}
	~IntVec3() {}
	IntVec3(const IntVec3& copyFrom);
	explicit IntVec3(int initialX, int initialY, int initialZ);

	IntVec3 operator+(const IntVec3& vecToAdd) const;

	void SetFromText(const char* text);
};
