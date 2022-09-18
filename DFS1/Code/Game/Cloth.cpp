#include <vector>
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Game/Cloth.hpp"
#include "Game/Game.hpp"

constexpr float pointRadius = 0.5f;
constexpr float lineThickness = 0.25f;
//const Vec2 startPos = Vec2(50.f, 75.f);
constexpr float errorRoom = 0.3f;
constexpr int TOTAL_NUM_ITERATIONS = 2;
constexpr float clothTotalMass = 2000.f;
constexpr float impulseInterval = 2.f;

extern Renderer* g_theRenderer;

Cloth::Cloth(Game* game, IntVec2 pointGrid, Vec2 linkLength, ClothMassType weightType)
	:m_game(game), m_gridCoords(pointGrid), m_linkLength(linkLength)
{
	m_clothParticles.reserve((size_t)pointGrid.x * pointGrid.y);
	
	InitializeParticles(weightType);
	InitializeConstraints();

	for (int i = 0; i < m_gridCoords.x; i++)
	{
		if(i % 10 == 0)
			m_clothParticles[i].m_isPinned = true;

		if (i == m_gridCoords.x - 1)
			m_clothParticles[i].m_isPinned = true;
	}

	std::string textureFile = g_gameConfigBlackboard.GetValue("clothTexture", "");
	m_texture = g_theRenderer->CreateOrGetTextureFromFile(textureFile.c_str());
	m_gravity = -400.f;
}

void Cloth::Update(float deltaSeconds)
{
	//IdentifyBadConstraints(deltaSeconds);
	UpdateParticles(deltaSeconds);
	SatisfyConstraints();
}

void Cloth::Render() const
{
	if (m_game->m_renderClothTexture)
	{
		RenderCloth();
	}

	if (m_game->m_renderClothGrid)
	{
		RenderClothStructureGrid();
	}

	DebugRender();
}

void Cloth::BreakConstraintsWithNeighbours(Particle* referencePoint)
{
	for (int i = 0; i < m_clothParticles.size(); i++)
	{
		if (&m_clothParticles[i] == referencePoint)
		{
			m_particleIndiciesWithBrokenVerticalConstraint.push_back(i);
		}
	}

	for (auto iter = m_verticalConstraints.begin(); iter != m_verticalConstraints.end();)
	{
		if (iter->particleA == referencePoint)
		{
			iter = m_verticalConstraints.erase(iter);
		}
		else
			++iter;
	}

	for (auto iter = m_horizontalConstraints.begin(); iter != m_horizontalConstraints.end();)
	{
		if (iter->particleA == referencePoint)
		{
			iter = m_horizontalConstraints.erase(iter);
		}
		else
			++iter;
	}
}

void Cloth::MovePoint(const Vec2& screenMousePos, Particle*& grabbedParticle)
{
	GrabAndMovePoint(screenMousePos, m_clothParticles, grabbedParticle);
}

void Cloth::CollideWithCircle(const Vec2& circleCenter, float circleRadius)
{
	for (int i = 0; i < m_clothParticles.size(); i++)
	{
		PushDiscOutOfDisc2D(m_clothParticles[i].m_currentPos, 0.f, circleCenter, circleRadius);
	}
}

void Cloth::CollideWithBox(const AABB2& collisionBox)
{
	for (int i = 0; i < m_clothParticles.size(); i++)
	{
		PushDiscOutOfAABB2D(m_clothParticles[i].m_currentPos, 0.6f, collisionBox);
	}
}

void Cloth::UpdateParticles(float deltaSeconds)
{
	for (int i = 0; i < m_clothParticles.size(); i++)
	{
		UpdateParticle(m_clothParticles[i], deltaSeconds);
	}
}

void Cloth::InitializeParticles(ClothMassType weightType)
{
	float topToBottomMassSplitFactor = 0.5f;
	if (weightType == ClothMassType::TOP_HEAVY)
		topToBottomMassSplitFactor = 0.75f;
	else if (weightType == ClothMassType::BOTTOM_HEAVY)
		topToBottomMassSplitFactor = 0.25f;

	float totalMassOfTopPoints = clothTotalMass * topToBottomMassSplitFactor;
	float totalMassOfBottomPoints = clothTotalMass - totalMassOfTopPoints;
	int numberOfTopPoints = (m_gridCoords.y / 2) * m_gridCoords.x;
	float massOfEachTopPoint = totalMassOfTopPoints / float(numberOfTopPoints);
	int numberOfBottomPoints = (m_gridCoords.x * m_gridCoords.y) - numberOfTopPoints;
	float massOfEachBottomPoint = totalMassOfBottomPoints / numberOfBottomPoints;
	Vec2 worldSize = m_game->m_worldSize;
	float xStart = (worldSize.x - ((m_gridCoords.x - 1) * m_linkLength.x)) / 2.f;
	float yStart = worldSize.y -((worldSize.y - ((m_gridCoords.y - 1) * m_linkLength.y)) / 2.f);
	Vec2 startPos(xStart, yStart);

	//initialize points
	for (int y = 0; y < m_gridCoords.y; y++)
	{
		for (int x = 0; x < m_gridCoords.x; x++)
		{
			Particle particle;
			particle.m_currentPos = startPos + Vec2(x * m_linkLength.x, -y * m_linkLength.y);
			particle.m_prevPos = particle.m_currentPos;
			particle.m_mass = (y < int(m_gridCoords.y / 2)) ? massOfEachTopPoint : massOfEachBottomPoint;
			m_clothParticles.push_back(particle);
		}
	}	
}

void Cloth::InitializeConstraints()
{
	//initialize stick constraints
	for (int y = 0; y < m_gridCoords.y; y++)
	{
		for (int x = 0; x < m_gridCoords.x; x++)
		{
			int indexOfAdjacentEastPoint = 0;
			int indexOfAdjacentSouthPoint = 0;
			//no east link for the last point
			if (x < m_gridCoords.x - 1)
			{
				//constraint from current point to the point on east of it.
				DistanceConstraint constraintA;
				constraintA.particleA = &m_clothParticles[GetIndexForPointFromGridCoordinates(IntVec2(x, y))];
				indexOfAdjacentEastPoint = GetIndexForPointFromGridCoordinates(IntVec2(x, y) + IntVec2(1, 0));
				if (indexOfAdjacentEastPoint < m_clothParticles.size())
				{
					constraintA.particleB = &m_clothParticles[indexOfAdjacentEastPoint];
					constraintA.restLength = m_linkLength.x - errorRoom;
					constraintA.originalRestLength = constraintA.restLength;
					m_horizontalConstraints.push_back(constraintA);
				}
			}

			//constraint from current point to the point on south of it.
			DistanceConstraint constraintB;
			constraintB.particleA = &m_clothParticles[GetIndexForPointFromGridCoordinates(IntVec2(x, y))];
			indexOfAdjacentSouthPoint = GetIndexForPointFromGridCoordinates(IntVec2(x, y) + IntVec2(0, 1));
			if (indexOfAdjacentSouthPoint < m_clothParticles.size())
			{
				constraintB.particleB = &m_clothParticles[indexOfAdjacentSouthPoint];
				constraintB.restLength = m_linkLength.y - errorRoom;
				constraintB.originalRestLength = constraintB.restLength;
				m_verticalConstraints.push_back(constraintB);
			}
		}
	}
}

void Cloth::SatisfyConstraints()
{
	for (int j = 0; j < TOTAL_NUM_ITERATIONS; j++)
	{
		for (int i = 0; i < m_horizontalConstraints.size(); i++)
		{
			SatisfyDistanceConstraint(m_horizontalConstraints[i]);
		}

		for (int i = 0; i < m_verticalConstraints.size(); i++)
		{
			SatisfyDistanceConstraint(m_verticalConstraints[i]);
		}
	}
}

int Cloth::GetIndexForPointFromGridCoordinates(const IntVec2& gridCoords) const
{
	return gridCoords.x + (gridCoords.y * m_gridCoords.x);
}

//void Cloth::MovePoints(float deltaSeconds)
//{
//	float drag = 0.01f;
//	Vec2 acceleration = m_gravity;
//	for (int i = 0; i < m_points.size(); i++)
//	{
//		Point& point = m_points[i];
//		if(point.m_isPinned)
//			continue;
//
//		Vec2 temp = point.m_currentPos;
//		point.m_currentPos += (point.m_currentPos - point.m_previousPos) * (1.f - drag) + (point.m_mass * acceleration * (1.f - drag) * deltaSeconds * deltaSeconds);
//		point.m_previousPos = temp;
//	}
//}

void Cloth::IdentifyBadPoints(float deltaSeconds)
{
	UNUSED(deltaSeconds);
	m_badPoints.clear();

	for (int y = 0; y < m_gridCoords.y; y++)
	{
		for (int x = 0; x < m_gridCoords.x; x++)
		{
			if (y < m_gridCoords.y - 1)
			{
				Particle& currentPoint = m_clothParticles[GetIndexForPointFromGridCoordinates(IntVec2(x, y))];
				Particle& pointSouthOfCurrentPoint = m_clothParticles[GetIndexForPointFromGridCoordinates(IntVec2(x, y + 1))];
				if(IsPointBad(&currentPoint, &pointSouthOfCurrentPoint))
				{
					m_badPoints.push_back(&pointSouthOfCurrentPoint);
				}
			}
		}
	}
}

void Cloth::AddImpulseToBadPoints(float deltaSeconds)
{
	m_impulseIntervalTimer += deltaSeconds;
	if (m_impulseIntervalTimer > impulseInterval /*&& m_badPoints.size() < 2*/)
	{
		Vec2 impulse(-10.f, 50.f);
		for (int i = 0; i < m_badPoints.size(); i++)
		{
			Particle* badPoint = m_badPoints[i];
			badPoint->m_currentPos += impulse * deltaSeconds;
			//badPoint->m_previousPos += impulse * deltaSeconds;
		}
		m_impulseIntervalTimer = 0.f;
	}
}

void Cloth::DebugRender() const
{
	if (m_game->m_debugRender)
	{
		std::vector<Vertex_PCU> verts;

		for (int i = 0; i < m_badPoints.size(); i++)
		{
			AddVertsForDisc2D(verts, m_badPoints[i]->m_currentPos, pointRadius * 2.f, Rgba8::RED);
		}

		for (int i = 0; i < m_badConstraints.size(); i++)
		{
			AddVertsForDisc2D(verts, m_badConstraints[i].particleA->m_currentPos, pointRadius * 1.5f, Rgba8(255, 148, 112, 255));
			AddVertsForDisc2D(verts, m_badConstraints[i].particleB->m_currentPos, pointRadius * 1.5f, Rgba8::RED);
			AddVertsForLineSegment2D(verts, m_badConstraints[i].particleA->m_currentPos, m_badConstraints[i].particleB->m_currentPos, lineThickness * 1.5f, Rgba8::RED);
		}

		g_theRenderer->BindTexture(nullptr);
		g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());


	}
}

void Cloth::RenderClothStructureGrid() const
{
	std::vector<Vertex_PCU> verts;
	for (int i = 0; i < m_clothParticles.size(); i++)
	{
		AddVertsForDisc2D(verts, m_clothParticles[i].m_currentPos, pointRadius, Rgba8::WHITE);
	}

	for (int i = 0; i < m_horizontalConstraints.size(); i++)
	{
		DistanceConstraint constraint = m_horizontalConstraints[i];
		AddVertsForLineSegment2D(verts, constraint.particleA->m_currentPos, constraint.particleB->m_currentPos, lineThickness, Rgba8::WHITE);
	}
	for (int i = 0; i < m_verticalConstraints.size(); i++)
	{
		DistanceConstraint constraint = m_verticalConstraints[i];
		AddVertsForLineSegment2D(verts, constraint.particleA->m_currentPos, constraint.particleB->m_currentPos, lineThickness, Rgba8::WHITE);
	}
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}

void Cloth::RenderCloth() const
{
	//uv of each quad on the cloth grid will be total X/Y length divide by number of quads in each axis
	float uvLengthX = 1.f / (m_gridCoords.x - 1);
	float uvLengthY = 1.f / (m_gridCoords.y - 1);
	std::vector<Vertex_PCU> verts;
	for (int y = 0; y < m_gridCoords.y - 1; y++)
	{
		for (int x = 0; x < m_gridCoords.x - 1; x++)
		{
			int topLeftParticleIndex = GetIndexForPointFromGridCoordinates(IntVec2(x, y));
			int topRightParticleIndex = GetIndexForPointFromGridCoordinates(IntVec2(x + 1, y));
			int bottomLeftParticleIndex = GetIndexForPointFromGridCoordinates(IntVec2(x, y + 1));
			int bottomRightParticleIndex = GetIndexForPointFromGridCoordinates(IntVec2(x + 1, y + 1));
			
			//uv's are mapped from bottom left to top right, but the cloth grid starts (0, 0) at top left and ends (1, 1) at bottom right
			//hence for uv's for the cloth, uv.x are mapped as usual, but y is flipped to get correct uv.y mapping
			Vec2 uvTopLeft(uvLengthX * x, 1.f - uvLengthY * y);
			Vec2 uvBottomRight(uvLengthX * (x + 1), 1.f - (uvLengthY * (y + 1)));

			if(DoesParticleHaveBrokenVerticalConstraints(topLeftParticleIndex)/* && DoesParticleHaveBrokenVerticalConstraints(topRightParticleIndex)*/)
				continue;
			else
			{
				AddVertsForQuad3D(verts, Vec3(m_clothParticles[topLeftParticleIndex].m_currentPos), Vec3(m_clothParticles[bottomLeftParticleIndex].m_currentPos),
					Vec3(m_clothParticles[bottomRightParticleIndex].m_currentPos), Vec3(m_clothParticles[topRightParticleIndex].m_currentPos),
					Rgba8::WHITE, AABB2(Vec2(uvTopLeft.x, uvBottomRight.y), Vec2(uvBottomRight.x, uvTopLeft.y)));
			}
		}
	}
	g_theRenderer->BindTexture(m_texture);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}

bool Cloth::IsPointBad(Particle* particleA, Particle* particleB) const
{
	float errorMargin = m_linkLength.y * 0.3f;
	float deltaY = particleA->m_currentPos.y - particleB->m_currentPos.y;
	if (deltaY < errorMargin)
		return true;

	return false;
}

void Cloth::IdentifyBadConstraints(float deltaSeconds)
{
	constexpr float correctionInterval = 0.5f;
	float deltaRestLengthIncrease = m_linkLength.y * 0.05f;
	//identify bad constraints, only vertical ones and then keep increasing their rest length until its resolved and then restore
	//to original length
	m_constraintCorrectionTimer += deltaSeconds;
	if (m_constraintCorrectionTimer > correctionInterval)
	{
		m_badConstraints.clear();
		for (int i = 0; i < m_verticalConstraints.size(); i++)
		{
			DistanceConstraint& constraint = m_verticalConstraints[i];
			if (IsPointBad(constraint.particleA, constraint.particleB))
			{
				m_badConstraints.push_back(constraint);
				constraint.restLength -= deltaRestLengthIncrease;
			}
			else if (constraint.restLength > constraint.originalRestLength + deltaRestLengthIncrease ||
				constraint.restLength < constraint.originalRestLength - deltaRestLengthIncrease)
			{
				constraint.restLength += deltaRestLengthIncrease;
			}
		}
		m_constraintCorrectionTimer = 0.f;
	}
}

bool Cloth::DoesParticleHaveBrokenVerticalConstraints(int index) const
{
	for (int i = 0; i < m_particleIndiciesWithBrokenVerticalConstraint.size(); i++)
	{
		if (m_particleIndiciesWithBrokenVerticalConstraint[i] == index)
			return true;
	}

	return false;
}

//void Cloth::GrabPointOnCloth(const Vec2& screenMousePos)
//{
//	constexpr float discCheckRadius = 2.f;
//
//	if (m_game->m_grabbedPoint == nullptr)
//	{
//		//check if any point on the cloth lies in a small circle around the current mouse pos
//		for (int i = 0; i < m_particles.size(); i++)
//		{
//			Particle& particle = m_particles[i];
//			if (IsPointInsideDisc2D(particle.m_currentPos, screenMousePos, discCheckRadius))
//			{
//				particle.m_currentPos = screenMousePos;
//				m_game->m_grabbedPoint = &particle;
//			}
//		}
//	}
//	else
//	{
//		m_game->m_grabbedPoint->m_currentPos = screenMousePos;
//	}
//}
