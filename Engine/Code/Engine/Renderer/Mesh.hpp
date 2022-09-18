#pragma once

class VertexBuffer;
class IndexBuffer;
class MeshBuilder;
class Renderer;

class Mesh
{
public:
	explicit Mesh(Renderer* renderer);
	~Mesh();
	bool UpdateFromBuilder(const MeshBuilder& builder);

	VertexBuffer* GetVertexBuffer() const;
	IndexBuffer* GetIndexBuffer() const;
	unsigned int GetElementCount() const;

private:
	Renderer* m_renderer = nullptr;
	VertexBuffer* m_vertexBuffer = nullptr;
	IndexBuffer* m_indexBuffer = nullptr;
	bool m_usesIndices = false;
	unsigned int m_elementCount = 0;
};