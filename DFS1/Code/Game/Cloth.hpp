#pragma once
#include "Game/ParticleSystem.hpp"

constexpr float distanceBetweenPointsOnX = 3.f;
constexpr float distanceBetweenPointsOnY = 3.f;

class Game;
class Texture;

enum class ClothMassType
{
	TOP_HEAVY,
	BOTTOM_HEAVY,
	UNIFORM
};

//struct Point
//{
//	Vec2 m_currentPos = Vec2::ZERO;
//	Vec2 m_previousPos = Vec2::ZERO;
//	bool m_isPinned = false;
//	int pointIndex = 0;
//	float m_mass = 0.f;
//	bool m_isBottomLinkBroken = false;
//};

//struct ClothParticle : public Particle
//{
//	bool m_isBottomLinkBroken = false;
//};

class Cloth : public ParticleSystem
{
public:
	Cloth(Game* game, IntVec2 pointGrid, Vec2 linkLength, ClothMassType weightType);
	void Update(float deltaSeconds) override;
	void Render() const override;
	void BreakConstraintsWithNeighbours(Particle* referencePoint);
	void MovePoint(const Vec2& screenMousePos, Particle*& grabbedParticle);
	void CollideWithCircle(const Vec2& circleCenter, float circleRadius);
	void CollideWithBox(const AABB2& collisionBox);

public:
	std::vector<Particle> m_clothParticles;
	std::vector<DistanceConstraint> m_horizontalConstraints;
	std::vector<DistanceConstraint> m_verticalConstraints;
	std::vector<DistanceConstraint> m_badConstraints;

protected:
	//const Vec2 m_gravity = Vec2(0.f, -150.f);
	Game* m_game = nullptr;
	IntVec2 m_gridCoords = IntVec2::ZERO;
	Vec2 m_linkLength = Vec2(distanceBetweenPointsOnX, distanceBetweenPointsOnY);
	std::vector<Particle*> m_badPoints;
	float m_impulseIntervalTimer = 0.f;
	float m_constraintCorrectionTimer = 0.f;
	Texture* m_texture = nullptr;
	std::vector<int> m_particleIndiciesWithBrokenVerticalConstraint;

protected:
	void UpdateParticles(float deltaSeconds);
	void InitializeParticles(ClothMassType weightType);
	void InitializeConstraints();
	void SatisfyConstraints() override;
	//void SatisfyMinDistanceConstraint(MinDistanceConstraint& constraint);
	int GetIndexForPointFromGridCoordinates(const IntVec2& gridCoords) const;
	//void MovePoints(float deltaSeconds);
	void IdentifyBadPoints(float deltaSeconds);
	void AddImpulseToBadPoints(float deltaSeconds);
	void DebugRender() const;
	void RenderClothStructureGrid() const;
	void RenderCloth() const;
	bool IsPointBad(Particle* particleA, Particle* particleB) const;
	void IdentifyBadConstraints(float deltaSeconds);
	bool DoesParticleHaveBrokenVerticalConstraints(int index) const;
};