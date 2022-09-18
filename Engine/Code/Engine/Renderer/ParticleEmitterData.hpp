#pragma once
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/Rgba8.hpp"

struct ParticleEmitterData
{
public:
	int m_maxParticles = 0;
	float m_particlesEmittedPerSecond = 0.f;
	FloatRange m_particleLifetime;
	FloatRange m_startSpeed;
	FloatRange m_startSize;
	FloatRange m_startRotationDegrees;
	Rgba8 m_startColor;
	Rgba8 m_endColor;
	int m_gravityScale = 0;
	//bool m_isGPUEmitter = false;
};