#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"

struct Vertex_PCU
{
public:
	Vec3 m_position;
	Rgba8 m_color;
	Vec2 m_uvTexCoords;

	Vertex_PCU() {}
	Vertex_PCU(Vec3 const& pos, Rgba8 const& color, Vec2 const& uvTexCoords);
};