#include "Game/Plant.hpp"
#include "Engine/Core/XmlUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"

constexpr float pointRadius = 0.5f;
constexpr float lineThickness = 0.25f;
constexpr int TOTAL_NUM_ITERATION = 1;

extern InputSystem* g_theInput;
extern Renderer* g_theRenderer;

Plant::Plant(Game* game, const Vec2& root)
	:m_game(game)
{
	RandomNumberGenerator rng;
	m_gravity = -50.f;
	m_particles.reserve(100);

	InitializeStem(90.f, root, true);
	int particleListSize = int(m_particles.size() - 1);
	InitializeBespokeBranchOne(root, rng.GetRandomFloatInRange(0.7f, 1.2f));

	AddDistanceConstraint(particleListSize + 1, 8, true);
	AddAngularConstraint(9, particleListSize + 1, 8);
	AddDistanceConstraint(particleListSize + 2, 8);

	particleListSize = int(m_particles.size() - 1);
	InitializeBespokeBranchTwo(root, rng.GetRandomFloatInRange(0.7f, 1.2f));

	AddDistanceConstraint(particleListSize + 1, 5, true);
	AddAngularConstraint(6, particleListSize + 1, 5);
	AddDistanceConstraint(particleListSize + 3, 5);
}

void Plant::Update(float deltaSeconds)
{
	UpdateParticles(deltaSeconds);
	SatisfyConstraints();
}

void Plant::Render() const
{
	RenderStructure();
}

void Plant::UpdateParticles(float deltaSeconds)
{
	for (int i = 0; i < m_particles.size(); i++)
	{
		UpdateParticle(m_particles[i], deltaSeconds);
	}
}

bool Plant::LoadXmlData(const char* path)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLError status = doc.LoadFile(path);
	GUARANTEE_OR_DIE(status == tinyxml2::XML_SUCCESS, "Failed to load plant points data file");
	tinyxml2::XMLElement* rootElement = doc.RootElement();
	tinyxml2::XMLElement* pointsChildElement = rootElement->FirstChildElement("Points");

	XmlElement* pointElement = pointsChildElement->FirstChildElement();
	while (pointElement)
	{
		Particle point;
		point.m_currentPos = ParseXmlAttribute(*pointElement, "pos", point.m_currentPos);
		point.m_prevPos = point.m_currentPos;
		point.m_isPinned = ParseXmlAttribute(*pointElement, "pinned", point.m_isPinned);
		m_particles.push_back(point);
		pointElement = pointElement->NextSiblingElement();
	}

	XmlElement* constraints = pointsChildElement->NextSiblingElement("Constraints");
	XmlElement* constraintElement = constraints->FirstChildElement();
	while (constraintElement)
	{
		DistanceConstraint constraint;
		int pointAIndex = ParseXmlAttribute(*constraintElement, "pointAIndex", 0);
		int pointBIndex = ParseXmlAttribute(*constraintElement, "pointBIndex", 0);
		AddDistanceConstraint(pointAIndex, pointBIndex);
		constraintElement = constraintElement->NextSiblingElement();
	}

	return true;
}

void Plant::InitializeStem(float angle, const Vec2& originPoint, bool root /*, std::vector<Particle>& particleList, std::vector<DistanceConstraint>& constraintList*/)
{
	constexpr float leafHalfWidth = 5.f;
	constexpr float leafLength = 30.f;
	constexpr int leafSubdivisions = 10;
	//const Vec2 leafSpineDirection = Vec2(0.f, 1.f);
	const Vec2 leafSpineDirection = Vec2(CosDegrees(angle), SinDegrees(angle));
	float subDivisionLength = leafLength / leafSubdivisions;

	Vec2 leftRoot = originPoint + (leafSpineDirection.GetRotated90Degrees() * -1.f * leafHalfWidth);
	Vec2 rightRoot = originPoint + (leafSpineDirection.GetRotated90Degrees() * 1.f * leafHalfWidth);
	std::vector<Vec2> spinePoints;
	for (int i = 0; i < leafSubdivisions; i++)
	{
		Vec2 newPoint = originPoint + leafSpineDirection * (i * subDivisionLength);
		spinePoints.push_back(newPoint);
	}

	int initialParticleListSize = int(m_particles.size());
	//left root and right root particles have to always be the first and seconds elements in the array.
	Particle leftRootParticle;
	leftRootParticle.m_currentPos = leftRoot;
	leftRootParticle.m_prevPos = leftRoot;
	leftRootParticle.m_isPinned = root;
	m_particles.push_back(leftRootParticle);
	Particle rightRootParticle;
	rightRootParticle.m_currentPos = rightRoot;
	rightRootParticle.m_prevPos = rightRoot;
	rightRootParticle.m_isPinned = root;
	m_particles.push_back(rightRootParticle);

	for (int i = 0; i < spinePoints.size(); i++)
	{
		Particle particle;
		particle.m_currentPos = spinePoints[i];
		particle.m_prevPos = spinePoints[i];
		if (i == 0)
			particle.m_isPinned = root;

		m_particles.push_back(particle);
	}

	//all constraints from left root to spine points
	for (int i = 0; i < spinePoints.size(); i++)
	{
		//left root particle is the first in the particles array & spine particles start from the 3rd element in the array.
		AddDistanceConstraint(initialParticleListSize, initialParticleListSize + 2 + i);
	}

	//all constraints from right root to spine points
	for (int i = 0; i < spinePoints.size(); i++)
	{
		//right root particle is the 2nd in the particles array & spine particles start from the 3rd element in the array.
		AddDistanceConstraint(initialParticleListSize + 1, initialParticleListSize + 2 + i);
	}

	for (int i = 0; i < spinePoints.size() - 1; i++)
	{
		AddDistanceConstraint(initialParticleListSize + 2 + i, initialParticleListSize + 2 + i + 1, true);
	}

}

void Plant::SatisfyConstraints()
{
	for (int j = 0; j < TOTAL_NUM_ITERATION; j++)
	{
		for (int i = 0; i < m_constraints.size(); i++)
		{
			SatisfyDistanceConstraint(m_constraints[i]);
		}

		for (int i = 0; i < m_angularConstraints.size(); i++)
		{
			SatisfyAngularConstraint(m_angularConstraints[i]);
		}
	}
}

void Plant::SatisfyAngularConstraint(AngularConstraint& constraint)
{
	Vec2 v1 = constraint.particleA->m_currentPos - constraint.commonParticle->m_currentPos;
	Vec2 v2 = constraint.particleB->m_currentPos - constraint.commonParticle->m_currentPos;
	Vec2 averageVector = (v1.GetNormalized() + v2.GetNormalized()).GetNormalized();

	float v1CorrectionDirection = (v1.GetOrientationDegrees() - averageVector.GetOrientationDegrees()) > 0 ? 1.f : -1.f;
	Vec2 correctedV1 = averageVector.GetRotatedDegrees((constraint.desiredAngleDegrees / 2.f) * v1CorrectionDirection) * v1.GetLength();
	Vec2 correctedV2 = averageVector.GetRotatedDegrees((constraint.desiredAngleDegrees / 2.f) * v1CorrectionDirection * -1.f) * v2.GetLength();

	constraint.particleA->m_currentPos = constraint.commonParticle->m_currentPos + correctedV1;
	constraint.particleB->m_currentPos = constraint.commonParticle->m_currentPos + correctedV2;
}

void Plant::RenderStructure() const
{
	std::vector<Vertex_PCU> verts;
	for (int i = 0; i < m_particles.size(); i++)
	{
		AddVertsForDisc2D(verts, m_particles[i].m_currentPos, pointRadius, Rgba8::WHITE);
	}

	if (!m_renderStructureOnly)
	{
		for (int i = 0; i < m_constraints.size(); i++)
		{
			DistanceConstraint constraint = m_constraints[i];
			AddVertsForLineSegment2D(verts, constraint.particleA->m_currentPos, constraint.particleB->m_currentPos, lineThickness, Rgba8::WHITE);
		}
	}
	for (int i = 0; i < m_constraintsToRender.size(); i++)
	{
		DistanceConstraint constraint = m_constraintsToRender[i];
		AddVertsForLineSegment2D(verts, constraint.particleA->m_currentPos, constraint.particleB->m_currentPos, lineThickness, Rgba8::GREEN);
	}
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}

void Plant::InitializeBespokeBranchOne(const Vec2& root, float scale)
{
	int startIndex = (int)m_particles.size() - 1;
	Particle particle1;
	particle1.m_currentPos = root + (Vec2(5.f, 30.f) * scale);
	particle1.m_prevPos = particle1.m_currentPos;
	m_particles.push_back(particle1);

	Particle particle2;
	particle2.m_currentPos = root + (Vec2(7.f, 35.f) * scale);
	particle2.m_prevPos = particle2.m_currentPos;
	m_particles.push_back(particle2);

	Particle particle3;
	particle3.m_currentPos = root + (Vec2(3.f, 35.f) * scale);
	particle3.m_prevPos = particle3.m_currentPos;
	m_particles.push_back(particle3);

	AddDistanceConstraint(startIndex + 1, startIndex + 2, true);

	AddDistanceConstraint(startIndex + 3, startIndex + 1, true);

	AddAngularConstraint(startIndex + 2, startIndex + 3, startIndex + 1);
}

void Plant::InitializeBespokeBranchTwo(const Vec2& root, float scale)
{
	int startIndex = (int)m_particles.size() - 1;
	Particle particle1;
	particle1.m_currentPos = root + (Vec2(-10.f, 25.f) * scale);
	particle1.m_prevPos = particle1.m_currentPos;
	m_particles.push_back(particle1);

	Particle particle2;
	particle2.m_currentPos = root + (Vec2(-7.f, 30.f) * scale);
	particle2.m_prevPos = particle2.m_currentPos;
	m_particles.push_back(particle2);

	Particle particle3;
	particle3.m_currentPos = root + (Vec2(-12.f, 30.f) * scale);
	particle3.m_prevPos = particle3.m_currentPos;
	m_particles.push_back(particle3);

	AddDistanceConstraint(startIndex + 1, startIndex + 2, true);

	AddDistanceConstraint(startIndex + 3, startIndex + 1, true);

	AddAngularConstraint(startIndex + 2, startIndex + 3, startIndex + 1);
}

void Plant::AddDistanceConstraint(int aIndex, int bIndex, bool addToRenderList)
{
	DistanceConstraint constraint;
	constraint.particleA = &m_particles[aIndex];
	constraint.particleB = &m_particles[bIndex];
	constraint.restLength = GetDistance2D(constraint.particleA->m_currentPos, constraint.particleB->m_currentPos);
	constraint.originalRestLength = constraint.restLength;
	m_constraints.push_back(constraint);
	if (addToRenderList)
	{
		m_constraintsToRender.push_back(constraint);
	}
}

void Plant::AddAngularConstraint(int aIndex, int bIndex, int commonIndex)
{
	AngularConstraint angularConstraint;
	angularConstraint.particleA = &m_particles[aIndex];
	angularConstraint.particleB = &m_particles[bIndex];
	angularConstraint.commonParticle = &m_particles[commonIndex];
	angularConstraint.desiredAngleDegrees = angularConstraint.desiredAngleDegrees = GetAngleDegreesBetweenVectors2D(angularConstraint.particleA->m_currentPos - angularConstraint.commonParticle->m_currentPos,
		angularConstraint.particleB->m_currentPos - angularConstraint.commonParticle->m_currentPos);
	m_angularConstraints.push_back(angularConstraint);
}

void Plant::MovePoint(const Vec2& screenMousePos, Particle*& grabbedParticle)
{
	GrabAndMovePoint(screenMousePos, m_particles, grabbedParticle);
}

