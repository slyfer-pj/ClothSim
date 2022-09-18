#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/Vertex_PNCU.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include <vector>

struct Mat44;

void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float uniformScaleXY, float rotationDegreesAboutZ, const Vec2& translationXY);
void TransformVertexArray3D(int numVerts, Vertex_PCU* verts, const Mat44& transform);
void TransformVertexArray3D(int numVerts, Vertex_PNCU* verts, const Mat44& transform);

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, const AABB2& localBounds, Rgba8 color);
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, const AABB2& localBounds, Rgba8 color, const Vec2& uvAtMins, const Vec2& uvAtMaxs);

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, const Capsule2& capsule, const Rgba8& color);
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, const Vec2& boneStart, const Vec2& boneEnd, float radius, const Rgba8& color);

void AddVertsForRing2D(std::vector<Vertex_PCU>& verts, const Vec2& ringCenter, float ringRadius, Rgba8 color, float ringThickness);
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, const Vec2& center, float radius, const Rgba8& color);

void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, const OBB2& box, const Rgba8& color);

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, const Vec2& start, const Vec2& end, float thickness, const Rgba8& color);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, const LineSegment2& lineSegment, float thickness, const Rgba8& color);
void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, const Vec2& tailPos, const Vec2& tipPos, float lineThickness, const Rgba8& color);

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight,
	const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3D(std::vector<Vertex_PNCU>& verts, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight,
	const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indices, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight,
	const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3D(std::vector<Vertex_PNCU>& verts, std::vector<unsigned int>& indices, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight,
	const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForRoundedQuad3D(std::vector<Vertex_PNCU>& vertexes, const Vec3& topLeft, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topRight,
	const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const Rgba8& color = Rgba8::WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForSphere(std::vector<Vertex_PCU>& verts, int longitudinalSlices, int latitudinalSlices, float radius = 0.5f, const Vec3& center = Vec3::ZERO, const Rgba8& color = Rgba8::WHITE);
void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color);
void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color);
void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, const Vec3& base, const Vec3& tip, float baseRadius, const Rgba8& color);


