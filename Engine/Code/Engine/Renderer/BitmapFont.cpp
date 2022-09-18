#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/MathUtils.hpp"

BitmapFont::BitmapFont(const char* fontFilePathNameWithNoExtension, Texture& fontTexture)
	:m_fontGlyphSpriteSheet(fontTexture, IntVec2(16, 16)),
	m_fontFilePathWithNoExtension(fontFilePathNameWithNoExtension)
{
}

Texture& BitmapFont::GetTexture()
{
	return m_fontGlyphSpriteSheet.GetTexture();
}

void BitmapFont::AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, const Vec2& textMins, float cellHeight, const std::string& text, const Rgba8& tint, float cellAspect, int maxGlyphsToDraw)
{
	for (int i = 0; i < text.size() && maxGlyphsToDraw > 0; i++)
	{
		Vec2 characterAABBLowerBound = Vec2(textMins.x + (i * cellAspect * cellHeight), textMins.y);
		Vec2 characterAABBUpperBound = Vec2(textMins.x + ((i + 1) * cellAspect * cellHeight), textMins.y + cellHeight);
		AABB2 characterSpriteUvs = m_fontGlyphSpriteSheet.GetSpriteUVs(text[i]);
		AddVertsForAABB2D(vertexArray, AABB2(characterAABBLowerBound, characterAABBUpperBound), tint, characterSpriteUvs.m_mins, characterSpriteUvs.m_maxs);
		maxGlyphsToDraw--;
	}
}

void BitmapFont::AddVertsForTextInBox2D(std::vector<Vertex_PCU>& vertexArray, const AABB2& box, float cellHeight, const std::string& text, const Rgba8& tint , float cellAspect , const Vec2& alignment , TextBoxMode mode , int maxGlyphsToDraw)
{
	Strings textStrings = SplitStringOnDelimiter(text, '\n');
	Vec2 boxDimensions = box.GetDimensions();
	
	std::string longestStringInTextGroup = GetLongestStringAmongstStrings(textStrings);
	float textGroupLongestStringWidth = GetTextWidth(cellHeight, longestStringInTextGroup, cellAspect);
	float textGroupHeight = textStrings.size() * cellHeight;
	float shrinkFactor = 1.f;
	if (mode == TextBoxMode::SHRINK_TO_FIT)
	{
		//adjust for greater height
		if (textGroupHeight > textGroupLongestStringWidth)
			shrinkFactor = boxDimensions.y / textGroupHeight;
		//adjust for greater
		else
			shrinkFactor = boxDimensions.x / textGroupLongestStringWidth;
		shrinkFactor = Clamp(shrinkFactor, 0.f, 1.f);
	}
	float textGroupMinY = (boxDimensions.y - (textGroupHeight * shrinkFactor)) * alignment.y;

	int glyphsToDraw = maxGlyphsToDraw;
	for (int i = 0; i < textStrings.size() && glyphsToDraw > 0; i++)
	{
		float textLineHeight = cellHeight * shrinkFactor;
		float textLineWidth = GetTextWidth(textLineHeight, textStrings[i], cellAspect);

		float leftPadding = (boxDimensions.x - textLineWidth) * alignment.x;
		float botPadding = textGroupMinY + (textLineHeight * (float(textStrings.size() - 1 - i)));

		float textLineMinX = box.m_mins.x + leftPadding;
		float textLineMinY = box.m_mins.y + botPadding;
		AddVertsForText2D(vertexArray, Vec2(textLineMinX, textLineMinY), textLineHeight, textStrings[i], tint, cellAspect, glyphsToDraw);
		glyphsToDraw -= (int)textStrings[i].length();
	}
}

float BitmapFont::GetTextWidth(float cellHeight, const std::string& text, float cellAspect)
{
	return (text.size() * cellHeight * cellAspect);
}

const std::string& BitmapFont::GetFontFilePath() const
{
	return m_fontFilePathWithNoExtension;
}

float BitmapFont::GetGlyphAspect(int glyphUnicode) const
{
	UNUSED(glyphUnicode);
	return 1.0f;
}

