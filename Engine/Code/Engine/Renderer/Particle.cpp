#include "Engine/Renderer/Particle.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Renderer/ParticleEmitter.hpp"

void Particle::Update(float deltaSeconds, const ParticleEmitterData& data)
{
	m_velocity += Vec3(0.f, 0.f, -10.f) * static_cast<float>(data.m_gravityScale) * deltaSeconds;
	m_position += m_velocity * deltaSeconds;
	m_age += deltaSeconds;
	Rgba8 color = Interpolate(data.m_startColor, data.m_endColor, GetNormalizedAge());
	color.GetAsFloats(m_color);
}

bool Particle::IsAlive() const
{
	return m_age < m_lifeTime;
}

float Particle::GetNormalizedAge() const
{
	return m_age / m_lifeTime;
}