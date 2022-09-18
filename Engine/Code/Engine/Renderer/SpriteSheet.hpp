#pragma once
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include <vector>

//class SpriteDefinition;

class SpriteSheet
{
public:
	explicit SpriteSheet(Texture& texture, const IntVec2& simpleGridLayout);
	Texture& GetTexture() const;
	IntVec2 GetSize() const { return m_size; }
	int GetNumSprites() const;
	const SpriteDefinition& GetSpriteDefinition(int spriteNum) const;
	void GetSpriteUVs(Vec2& out_UVsAtMin, Vec2& out_UVsAtMax, int spriteIndex) const;
	AABB2 GetSpriteUVs(int spriteIndex) const;

protected:
	IntVec2 m_size = IntVec2::ZERO;
	Texture& m_texture;
	std::vector<SpriteDefinition> m_spriteDefinitions;

private:
	void GenerateSpriteDefinitions(IntVec2 gridLayout);
	void ConvertSpriteBoundsToUVs(Vec2& lowerBound, Vec2& upperBound, IntVec2 textureDimensions);
};