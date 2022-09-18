#pragma once
#include "Game/ParticleSystem.hpp"

class Game;

struct AngularConstraint
{
	Particle* particleA;
	Particle* particleB;
	Particle* commonParticle;
	float desiredAngleDegrees = 0.f;
};

class Plant : public ParticleSystem
{
public:
	Plant(Game* game, const Vec2& root);
	void Update(float deltaSeconds) override;
	void Render() const override;
	void MovePoint(const Vec2& screenMousePos, Particle*& grabbedParticle);
	bool m_renderStructureOnly = true;

protected:
	Game* m_game = nullptr;
	std::vector<Particle> m_particles;
	std::vector<DistanceConstraint> m_constraints;
	//std::vector<Particle> m_angleConstraintParticles;
	std::vector<AngularConstraint> m_angularConstraints;
	std::vector<DistanceConstraint> m_constraintsToRender; //this vector is only to render the structure that has only the main plant structure

protected:
	void UpdateParticles(float deltaSeconds);
	bool LoadXmlData(const char* path);
	void InitializeStem(float angle, const Vec2& originPoint, bool root = false);
	void SatisfyConstraints() override;
	void SatisfyAngularConstraint(AngularConstraint& constraint);
	void RenderStructure() const;
	void InitializeBespokeBranchOne(const Vec2& root, float scale);
	void InitializeBespokeBranchTwo(const Vec2& root, float scale);
	void AddDistanceConstraint(int aIndex, int bIndex, bool addToRenderList = false);
	void AddAngularConstraint(int aIndex, int bIndex, int commonIndex);
};