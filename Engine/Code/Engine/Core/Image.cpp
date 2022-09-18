#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Core/Image.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"

Image::Image(const char* imageFilePath)
{
	m_imageFilePath = imageFilePath;

	int bytesPerTexel = 0;
	int numComponentsRequested = 0;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* texelData = stbi_load(imageFilePath, &m_dimension.x, &m_dimension.y, &bytesPerTexel, numComponentsRequested);
	GUARANTEE_OR_DIE(texelData, Stringf("Failed to load image \"%s\"", imageFilePath));
	StoreTexelData(texelData, bytesPerTexel);
	stbi_image_free(texelData);
}

Image::Image(IntVec2 size, Rgba8 color)
	:m_dimension(size)
{
	m_rgbaTexels.assign(size.x * size.y, color);
}

const std::string& Image::GetImageFilePath() const
{
	return m_imageFilePath;
}

IntVec2 Image::GetDimensions() const
{
	return m_dimension;
}

const void* Image::GetRawData() const
{
	return m_rgbaTexels.data();
}

Rgba8 Image::GetTexelColor(const IntVec2& texelCoords) const
{
	int texelIndex = texelCoords.x + (texelCoords.y * m_dimension.x);
	return m_rgbaTexels[texelIndex];
}

void Image::SetTexelColor(const IntVec2& texelCoords, const Rgba8& newColor)
{
	int texelIndex = texelCoords.x + (texelCoords.y * m_dimension.x);
	m_rgbaTexels[texelIndex] = newColor;
}

void Image::StoreTexelData(unsigned char* texelData, int bytesPerTexel)
{
	m_rgbaTexels.reserve(m_dimension.x * m_dimension.y);
	int incrementSize = bytesPerTexel == 3 ? 3 : 4;
	int imageSize = m_dimension.x * m_dimension.y;

	for (int i = 0; i < imageSize; i++)
	{
		int pointerIndex = (i * incrementSize);
		unsigned char r = *(texelData + pointerIndex);
		unsigned char g = *(texelData + pointerIndex + 1);
		unsigned char b = *(texelData + pointerIndex + 2);

		unsigned char a = 255;
		if (bytesPerTexel == 4)
			a = *(texelData + pointerIndex + 3);

		m_rgbaTexels.push_back(Rgba8(r, g, b, a));
	}
}
