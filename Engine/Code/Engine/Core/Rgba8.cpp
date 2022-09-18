#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/MathUtils.hpp"

const Rgba8 Rgba8::RED = Rgba8(255, 0, 0, 255);
const Rgba8 Rgba8::GREEN = Rgba8(0, 255, 0, 255);
const Rgba8 Rgba8::BLUE = Rgba8(0, 0, 255, 255);
const Rgba8 Rgba8::CYAN = Rgba8(0, 255, 255, 255);
const Rgba8 Rgba8::MAGENTA = Rgba8(255, 0, 255, 255);
const Rgba8 Rgba8::YELLOW = Rgba8(255, 255, 0, 255);
const Rgba8 Rgba8::WHITE = Rgba8(255, 255, 255, 255);
const Rgba8 Rgba8::BLACK = Rgba8(0, 0, 0, 255);
const Rgba8 Rgba8::GREY = Rgba8(128, 128, 128, 255);

Rgba8::Rgba8(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void Rgba8::GetAsFloats(float* colorAsFloats) const
{
	*colorAsFloats = NormalizeByte(r);
	*(colorAsFloats + 1) = NormalizeByte(g);
	*(colorAsFloats + 2) = NormalizeByte(b);
	*(colorAsFloats + 3) = NormalizeByte(a);
}

void Rgba8::SetFromFloats(const float* colorAsFloat)
{
	r = DenormalizeByte(*colorAsFloat);
	g = DenormalizeByte(*(colorAsFloat + 1));
	b = DenormalizeByte(*(colorAsFloat + 2));
	a = DenormalizeByte(*(colorAsFloat + 3));
}

void Rgba8::SetFromText(const char* text)
{
	Strings subStrings = SplitStringOnDelimiter(text, ',');
	r = (unsigned char)atoi(subStrings[0].c_str());
	g = (unsigned char)atoi(subStrings[1].c_str());
	b = (unsigned char)atoi(subStrings[2].c_str());

	if (subStrings.size() == 4)
		a = (unsigned char)atoi(subStrings[3].c_str());
	else
		a = 255;
}

std::string Rgba8::ToXMLString() const
{
	//string formatted such that xml parsing methods can decipher it
	return Stringf("%d,%d,%d,%d", r, g, b, a);
}

const Rgba8 Rgba8::operator*(float scaleFactor) const
{
	return Rgba8(unsigned char(r * scaleFactor), unsigned char(g * scaleFactor), unsigned char(b * scaleFactor), a);
}

const Rgba8 Rgba8::operator+(unsigned char addValue) const
{
	return Rgba8(r + addValue, g + addValue, b + addValue, a);
}

bool Rgba8::operator!=(const Rgba8& compare) const
{
	return (r != compare.r || g != compare.g || b != compare.b || a != compare.a);
}

bool Rgba8::operator==(const Rgba8& compare) const
{
	return (r == compare.r && g == compare.g && b == compare.b);
}

Rgba8 Interpolate(const Rgba8& a, const Rgba8& b, float fraction)
{
	Rgba8 output;
	output.r = DenormalizeByte(Interpolate(NormalizeByte(a.r), NormalizeByte(b.r), fraction));
	output.g = DenormalizeByte(Interpolate(NormalizeByte(a.g), NormalizeByte(b.g), fraction));
	output.b = DenormalizeByte(Interpolate(NormalizeByte(a.b), NormalizeByte(b.b), fraction));
	output.a = DenormalizeByte(Interpolate(NormalizeByte(a.a), NormalizeByte(b.a), fraction));
	return output;
}
