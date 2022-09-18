#pragma once
#include <string>

struct Rgba8 
{
public:
	unsigned char r = 255;
	unsigned char g = 255;
	unsigned char b = 255;
	unsigned char a = 255;

	static const Rgba8 RED;
	static const Rgba8 GREEN;
	static const Rgba8 BLUE;
	static const Rgba8 CYAN;
	static const Rgba8 MAGENTA;
	static const Rgba8 YELLOW;
	static const Rgba8 WHITE;
	static const Rgba8 BLACK;
	static const Rgba8 GREY;

	Rgba8() {}
	explicit Rgba8(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	void GetAsFloats(float* colorAsFloat) const;
	void SetFromFloats(const float* colorAsFloat);
	void SetFromText(const char* text);
	std::string ToXMLString() const;

	bool operator==(const Rgba8& compare) const;
	bool operator!=(const Rgba8& compare) const;
	const Rgba8 operator*(float scaleFactor) const;
	const Rgba8 operator+(unsigned char addValue) const;
};

Rgba8 Interpolate(const Rgba8& a, const Rgba8& b, float fraction);