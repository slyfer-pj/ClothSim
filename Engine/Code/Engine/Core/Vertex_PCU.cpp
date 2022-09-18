#include "Engine/Core/Vertex_PCU.hpp"

Vertex_PCU::Vertex_PCU(Vec3 const& pos, Rgba8 const& color, Vec2 const& uvTexCoords)
{
	this->m_position = pos;
	this->m_color = color;
	this->m_uvTexCoords = uvTexCoords;
}