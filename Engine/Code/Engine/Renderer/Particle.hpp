#pragma once
#include "Engine/Math/Vec3.hpp"

struct ParticleEmitterData;

struct Particle
{
public:
	void Update(float deltaSeconds, const ParticleEmitterData& data);
	bool IsAlive() const;

public:
	Vec3 m_position;
	Vec3 m_velocity;
	float m_color[4] = { 0.f };
	float m_rotation = 0.f;
	float m_lifeTime = 0.f;
	float m_age = 0.f;

private:
	float GetNormalizedAge() const;
};