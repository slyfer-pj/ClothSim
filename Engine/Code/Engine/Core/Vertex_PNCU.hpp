#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Rgba8.hpp"

//------------------------------------------------------------------------------------------------
struct Vertex_PNCU
{
	Vertex_PNCU() = default;
	Vertex_PNCU(Vec3 const& position, Vec3 const& normal, Rgba8 const& color, Vec2 const& uvTexCoords);
	Vertex_PNCU(float px, float py, float pz, float nx, float ny, float nz, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v);

	Vec3	m_position;
	Vec3	m_normal;
	Rgba8	m_color;
	Vec2	m_uvTexCoords;
};

