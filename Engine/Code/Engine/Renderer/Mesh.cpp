#include "Engine/Renderer/Mesh.hpp"
#include "Engine/Renderer/MeshBuilder.hpp"
#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/IndexBuffer.hpp"
#include "Engine/Renderer/Renderer.hpp"

Mesh::Mesh(Renderer* renderer)
	:m_renderer(renderer)
{
}

Mesh::~Mesh()
{
	delete m_vertexBuffer;
	delete m_indexBuffer;
}

bool Mesh::UpdateFromBuilder(const MeshBuilder& builder)
{
	if (builder.GetNumIndices() > 0)
	{
		m_usesIndices = true;
		m_elementCount = builder.GetNumIndices();
	}
	else
	{
		m_usesIndices = false;
		m_elementCount = builder.GetNumVertices();
	}

	size_t vertexBufferSize = builder.GetSizeOfVertex() * builder.GetNumVertices();
	size_t indexBufferSize = builder.GetSizeOfIndex() * builder.GetNumIndices();
	if (!m_vertexBuffer)
	{
		m_vertexBuffer = m_renderer->CreateVertexBuffer(vertexBufferSize, (unsigned int)builder.GetSizeOfVertex());
	}
	if (!m_indexBuffer)
	{
		m_indexBuffer = m_renderer->CreateIndexBuffer(indexBufferSize);
	}

	m_renderer->CopyCPUToGPU(builder.GetVerticesData().data(), vertexBufferSize, m_vertexBuffer);
	m_renderer->CopyCPUToGPU(builder.GetIndicesData().data(), indexBufferSize, m_indexBuffer);

	return true;
}

VertexBuffer* Mesh::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

IndexBuffer* Mesh::GetIndexBuffer() const
{
	return m_indexBuffer;
}

unsigned int Mesh::GetElementCount() const
{
	return m_elementCount;
}

