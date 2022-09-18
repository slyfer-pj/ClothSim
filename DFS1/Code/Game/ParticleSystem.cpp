#include "Game/ParticleSystem.hpp"
#include "Engine/Math/MathUtils.hpp"

void ParticleSystem::ChangeHorizontalForceBy(float changeAmount)
{
	m_horizontalForce += changeAmount;
}

void ParticleSystem::GrabAndMovePoint(const Vec2& screenMousePos, std::vector<Particle>& particles, Particle*& grabbedParticle)
{
	constexpr float discCheckRadius = 2.f;
	if (grabbedParticle == nullptr)
	{
		//check if any point lies in a small circle around the current mouse pos
		for (int i = 0; i < particles.size(); i++)
		{
			Particle& particle = particles[i];
			if (IsPointInsideDisc2D(particle.m_currentPos, screenMousePos, discCheckRadius))
			{
				particle.m_currentPos = screenMousePos;
				grabbedParticle = &particle;
				return;
			}
		}
	}
	else
	{
		grabbedParticle->m_currentPos = screenMousePos;
	}
}

void ParticleSystem::UpdateParticle(Particle& particleToUpdate, float deltaSeconds)
{
	float drag = 0.01f;
	Vec2 acceleration = Vec2(m_horizontalForce, m_gravity);
	if (particleToUpdate.m_isPinned)
		return;

	Vec2 temp = particleToUpdate.m_currentPos;
	particleToUpdate.m_currentPos += (particleToUpdate.m_currentPos - particleToUpdate.m_prevPos) * (1.f - drag) + (acceleration * deltaSeconds * deltaSeconds);
	particleToUpdate.m_prevPos = temp;
}

void ParticleSystem::SatisfyDistanceConstraint(DistanceConstraint& constraint)
{
	float invMassPointA = 1.f / constraint.particleA->m_mass;
	float invMassPointB = 1.f / constraint.particleB->m_mass;
	Vec2 vectorAB = constraint.particleB->m_currentPos - constraint.particleA->m_currentPos;
	float vectorLength = GetDistance2D(constraint.particleA->m_currentPos, constraint.particleB->m_currentPos);
	float excessPercent = (vectorLength - constraint.restLength) / (vectorLength * (invMassPointA + invMassPointB));
	if (!constraint.particleA->m_isPinned)
	{
		constraint.particleA->m_currentPos += vectorAB * invMassPointA * excessPercent;
	}
	if (!constraint.particleB->m_isPinned)
	{
		constraint.particleB->m_currentPos -= vectorAB * invMassPointB * excessPercent;
	}

}

