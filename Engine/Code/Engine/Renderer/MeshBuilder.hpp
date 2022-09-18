#pragma once
#include <vector>
#include "Engine/Core/Vertex_PNCU.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/Mat44.hpp"

struct mesh_import_options
{
	Mat44 m_transform;
	float m_scale = 1.f;
	bool m_reverseWindingOrder = false;
	bool m_invertV = false;	//if uv (0, 0) starts from top left instead of bot left
};

//is the CPU representation of the verts and indices, aka, CPU Mesh
class MeshBuilder
{
public:
	bool ImportFromOBJFile(const char* filepath, const mesh_import_options& importOptions);

	unsigned int GetNumVertices() const;
	const std::vector<Vertex_PNCU> GetVerticesData() const;
	size_t GetSizeOfVertex() const;
	unsigned int GetNumIndices() const;
	const std::vector<unsigned int> GetIndicesData() const;
	size_t GetSizeOfIndex() const;

	bool IsCPUMeshDirty() const;
	void SetCPUMeshDirty(bool dirty);

	void TransformVerts(const Mat44& transform);
	void ReverseWindingOrder();
	void InvertV();
	void ApplyScale(float scale);

	void Save(const char* filepath);
	void Load(const char* filepath);

private:
	std::string MakeValuesStringFromTokens(const Strings& tokens);

private:
	std::vector<Vertex_PNCU> m_vertices;
	std::vector<unsigned int> m_indices;
	bool m_isCPUMeshDirty = false;
};