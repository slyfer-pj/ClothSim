#include <d3d11.h>
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Core/Vertex_PCU.hpp"

VertexBuffer::VertexBuffer(size_t size, unsigned int stride)
	:m_size(size), m_stride(stride)
{
}

VertexBuffer::~VertexBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}

unsigned int VertexBuffer::GetStride() const
{
	return m_stride;
}
