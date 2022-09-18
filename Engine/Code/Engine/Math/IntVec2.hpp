#pragma once

struct IntVec2
{
public:
	int x = 0;
	int y = 0;

	static IntVec2 const ZERO;
	static IntVec2 const ONE;

public:
	IntVec2() {}
	~IntVec2() {}
	IntVec2(const IntVec2& copyFrom);
	explicit IntVec2(int initialX, int initialY);

	float GetLength() const;
	int GetLengthSquared() const;
	int GetTaxicabLength() const;
	const IntVec2 GetRotated90Degrees() const;
	const IntVec2 GetRotatedMinus90Degrees() const;
	float GetOrientationDegrees() const;
	float GetOrientationRadians() const;

	void Rotate90Degrees();
	void RotateMinus90Degrees();

	void SetFromText(const char* text);

	const IntVec2 operator+(const IntVec2& intVecToAdd) const;
	const IntVec2 operator-(const IntVec2& intVecToSubtract) const;
	const bool operator==(const IntVec2& intVecToCompare) const;
	const bool operator!=(const IntVec2& intVecToCompare) const;
};