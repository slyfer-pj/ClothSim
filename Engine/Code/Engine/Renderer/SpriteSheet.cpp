#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Math/MathUtils.hpp"

SpriteSheet::SpriteSheet(Texture& texture, const IntVec2& simpleGridLayout)
	:m_texture(texture), m_size(simpleGridLayout)
{
	GenerateSpriteDefinitions(simpleGridLayout);
}

Texture& SpriteSheet::GetTexture() const
{
	return m_texture;
}

int SpriteSheet::GetNumSprites() const
{
	return (int)m_spriteDefinitions.size();
}

const SpriteDefinition& SpriteSheet::GetSpriteDefinition(int spriteNum) const
{
	return m_spriteDefinitions[spriteNum];
}

void SpriteSheet::GetSpriteUVs(Vec2& out_UVsAtMin, Vec2& out_UVsAtMax, int spriteIndex) const
{
	m_spriteDefinitions[spriteIndex].GetUVs(out_UVsAtMin, out_UVsAtMax);
}

AABB2 SpriteSheet::GetSpriteUVs(int spriteIndex) const
{
	return m_spriteDefinitions[spriteIndex].GetUVs();
}

void SpriteSheet::GenerateSpriteDefinitions(IntVec2 gridLayout)
{
	int spriteIndex = 0;
	IntVec2 textureDimensions = m_texture.GetDimensions();
	float singleSpriteWidth = (float)(textureDimensions.x / gridLayout.x);
	float singleSpriteHeight = (float)(textureDimensions.y / gridLayout.y);
	for (int y = gridLayout.y - 1; y >= 0; y--)
	{
		for (int x = 0; x < gridLayout.x; x++)
		{
			Vec2 spriteLowerBound = Vec2(singleSpriteWidth * x, singleSpriteHeight * y);
			Vec2 spriteUpperBound = spriteLowerBound + Vec2(singleSpriteWidth, singleSpriteHeight);
			ConvertSpriteBoundsToUVs(spriteLowerBound, spriteUpperBound, textureDimensions);
			SpriteDefinition spriteDef = SpriteDefinition(*this, spriteIndex++, spriteLowerBound, spriteUpperBound);
			m_spriteDefinitions.push_back(spriteDef);
		}
	}
}

void SpriteSheet::ConvertSpriteBoundsToUVs(Vec2& lowerBound, Vec2& upperBound, IntVec2 textureDimensions)
{
	float uCorrection = 1.f / (textureDimensions.x * 100.f);
	float vCorrection = 1.f / (textureDimensions.y * 100.f);

	lowerBound.x = RangeMapClamped(lowerBound.x, 0.f, (float)textureDimensions.x, 0.f, 1.f) + uCorrection;
	lowerBound.y = RangeMapClamped(lowerBound.y, 0.f, (float)textureDimensions.y, 0.f, 1.f) + vCorrection;

	upperBound.x = RangeMapClamped(upperBound.x, 0.f, (float)textureDimensions.x, 0.f, 1.f) - uCorrection;
	upperBound.y = RangeMapClamped(upperBound.y, 0.f, (float)textureDimensions.y, 0.f, 1.f) - vCorrection;
}

