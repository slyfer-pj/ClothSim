#pragma once

class SpriteSheet;
class SpriteDefinition;

enum class SpriteAnimPlaybackType
{
	ONCE,
	LOOP,
	PINGPONG
};

class SpriteAnimDefinition
{
public:
	SpriteAnimDefinition(const SpriteSheet& spriteSheet, int startSpriteIndex, int endSpriteIndex, float durationSeconds,
		SpriteAnimPlaybackType playbackType = SpriteAnimPlaybackType::LOOP);
	const SpriteDefinition& GetSpriteDefAtTime(float seconds) const;
	float GetDuration() const { return m_durationSeconds; }

private:
	const SpriteSheet& m_spriteSheet;
	int m_startSpriteIndex = -1;
	int m_endSpriteIndex = -1;
	float m_durationSeconds = 1.f;
	SpriteAnimPlaybackType m_playbackType = SpriteAnimPlaybackType::LOOP;
};