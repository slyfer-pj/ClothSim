#pragma once
#include <string>
#include <vector>
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/Rgba8.hpp"

class Image
{
	friend class Renderer;
public:
	Image(const char* imageFilePath);
	Image(IntVec2 size, Rgba8 color);
	const std::string& GetImageFilePath() const;
	IntVec2 GetDimensions() const;
	const void* GetRawData() const;
	Rgba8 GetTexelColor(const IntVec2& texelCoords) const;
	void SetTexelColor(const IntVec2& texelCoords, const Rgba8& newColor);

private:
	std::string m_imageFilePath;
	IntVec2 m_dimension = IntVec2::ZERO;
	std::vector<Rgba8> m_rgbaTexels;

private:
	void StoreTexelData(unsigned char* texelData, int bytesPerTexel);
};