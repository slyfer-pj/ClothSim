#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

SpriteAnimDefinition::SpriteAnimDefinition(const SpriteSheet& spriteSheet, int startSpriteIndex, int endSpriteIndex, float durationSeconds, SpriteAnimPlaybackType playbackType)
	:m_spriteSheet(spriteSheet),
	 m_startSpriteIndex(startSpriteIndex),
	 m_endSpriteIndex(endSpriteIndex),
	 m_durationSeconds(durationSeconds),
	 m_playbackType(playbackType)
{

}

const SpriteDefinition& SpriteAnimDefinition::GetSpriteDefAtTime(float seconds) const
{
	int numOfFramesThatRepeat = 0;
	if (m_playbackType == SpriteAnimPlaybackType::PINGPONG)
		numOfFramesThatRepeat = (m_endSpriteIndex - m_startSpriteIndex) * 2;
	else
		numOfFramesThatRepeat = m_endSpriteIndex - m_startSpriteIndex + 1;

	if (m_playbackType == SpriteAnimPlaybackType::ONCE && seconds >= m_durationSeconds)
		return m_spriteSheet.GetSpriteDefinition(m_endSpriteIndex);

	int frameIndexInRepititionSet = 0;
	int indexOfSpriteToRender = 0;
	if (m_durationSeconds > 0.f)
	{
		seconds = fmodf(seconds, m_durationSeconds);
		frameIndexInRepititionSet = static_cast<int>((seconds / m_durationSeconds) * numOfFramesThatRepeat);
	}

	if (m_playbackType == SpriteAnimPlaybackType::LOOP)
		indexOfSpriteToRender = m_startSpriteIndex + frameIndexInRepititionSet;
	else
	{
		if (frameIndexInRepititionSet <= (numOfFramesThatRepeat / 2))
			indexOfSpriteToRender = m_startSpriteIndex + frameIndexInRepititionSet;
		else
			indexOfSpriteToRender = m_endSpriteIndex - (frameIndexInRepititionSet % (numOfFramesThatRepeat / 2));
	}

	return m_spriteSheet.GetSpriteDefinition(indexOfSpriteToRender);
}
