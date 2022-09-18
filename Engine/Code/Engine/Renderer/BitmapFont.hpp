#pragma once
#include <vector>
#include <string>
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

class Texture;
struct Vec2;
struct Vertex_PCU;

enum class TextBoxMode
{
	SHRINK_TO_FIT,
	OVERRUN
};

class BitmapFont
{
	friend class Renderer;

private:
	BitmapFont(const char* fontFilePathNameWithNoExtension, Texture& fontTexture);

public:
	Texture& GetTexture();
	void AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, const Vec2& textMins, float cellHeight, const std::string& text,
		const Rgba8& tint = Rgba8::WHITE, float cellAspect = 1.f, int maxGlyphsToDraw = INT_MAX);
	void AddVertsForTextInBox2D(std::vector<Vertex_PCU>& vertexArray, const AABB2& box, float cellHeight, const std::string& text,
		const Rgba8& tint = Rgba8::WHITE, float cellAspect = 1.f, const Vec2& alignment = Vec2(0.5f, 0.5f),
		TextBoxMode mode = TextBoxMode::SHRINK_TO_FIT, int maxGlyphsToDraw = INT_MAX);
	float GetTextWidth(float cellHeight, const std::string& text, float cellAspect = 1.f);
	const std::string& GetFontFilePath() const;

protected:
	float GetGlyphAspect(int glyphUnicode) const;

protected:
	std::string m_fontFilePathWithNoExtension;
	SpriteSheet m_fontGlyphSpriteSheet;
};