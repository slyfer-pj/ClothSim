#pragma once
#include "Engine/Math/Vec2.hpp"
#include <vector>

struct Particle
{
	Vec2 m_currentPos = Vec2::ZERO;
	Vec2 m_prevPos = Vec2::ZERO;
	float m_mass = 1.f;
	bool m_isPinned = false;
};

struct DistanceConstraint
{
	Particle* particleA = nullptr;
	Particle* particleB = nullptr;
	float restLength = 0.f;
	float originalRestLength = 0.f;
};

class ParticleSystem
{
public:
	ParticleSystem() = default;
	virtual void Update(float deltaSeconds) = 0;
	virtual void Render() const = 0;
	void ChangeHorizontalForceBy(float changeAmount);
	float GetCurrentHorizontalForce() const { return m_horizontalForce; };

protected:
	float m_horizontalForce = 0.f;
	float m_gravity = 0.f;

protected:
	void GrabAndMovePoint(const Vec2& screenMousePos, std::vector<Particle>& particles, Particle*& grabbedParticle);
	void UpdateParticle(Particle& particleToUpdate, float deltaSeconds);
	virtual void SatisfyConstraints() = 0;
	void SatisfyDistanceConstraint(DistanceConstraint& constraint);

};