#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Texture.hpp"

class SpriteSheet;

class SpriteDefinition
{
public:
	explicit SpriteDefinition(const SpriteSheet& spriteSheet, int spriteIndex, const Vec2& uvAtMins, const Vec2& uvAtMaxs);
	void GetUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs) const;
	AABB2 GetUVs() const;
	const SpriteSheet& GetSpriteSheet() const;
	Texture& GetTexture() const;
	float GetAspect() const;

protected:
	const SpriteSheet& m_spriteSheet;
	int m_spriteIndex = -1;
	Vec2 m_uvAtMins = Vec2::ZERO;
	Vec2 m_uvAtMaxs = Vec2::ONE;
};