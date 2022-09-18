#include "Engine/Core/Vertex_PNCU.hpp"

Vertex_PNCU::Vertex_PNCU(Vec3 const& position, Vec3 const& normal, Rgba8 const& color, Vec2 const& uvTexCoords)
	: m_position(position)
	, m_normal(normal)
	, m_color(color)
	, m_uvTexCoords(uvTexCoords)
{}

Vertex_PNCU::Vertex_PNCU(float px, float py, float pz, float nx, float ny, float nz, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float u, float v)
	: m_position(px, py, pz)
	, m_normal(nx, ny, nz)
	, m_color(r, g, b, a)
	, m_uvTexCoords(u, v)
{}