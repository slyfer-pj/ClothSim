#include "Engine/Renderer/MeshBuilder.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/VertexUtils.hpp"

bool MeshBuilder::ImportFromOBJFile(const char* filepath, const mesh_import_options& importOptions)
{
	std::string bufferString;
	Strings entries;
	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::vector<std::string> faces;

	FileReadToString(bufferString, filepath);
	entries = SplitStringOnDelimiter(bufferString, '\n');

	for (int i = 0; i < entries.size(); i++)
	{
		Strings tokens = SplitStringOnDelimiter(entries[i], ' ');
		//found positions
		if (tokens[0] == "v")
		{
			Vec3 pos;
			pos.SetFromText(MakeValuesStringFromTokens(tokens).c_str());
			positions.push_back(pos);
		}
		//found normal
		else if (tokens[0] == "vn")
		{
			Vec3 normal;
			normal.SetFromText(MakeValuesStringFromTokens(tokens).c_str());
			normals.push_back(normal);
		}
		//found uv
		else if (tokens[0] == "vt")
		{
			Vec2 uv;
			uv.SetFromText(MakeValuesStringFromTokens(tokens).c_str());
			uvs.push_back(uv);
		}
		//found face
		else if (tokens[0] == "f")
		{
			int numberOfClusters = 0;
			int baseIndex = (int)m_vertices.size();
			for (int j = 0; j < tokens.size(); j++)
			{
				if (tokens[j].find('/') != std::string::npos)
				{
					numberOfClusters++;
					Strings listIndices = SplitStringOnDelimiter(tokens[j], '/');
					Vertex_PNCU vert;
					vert.m_position = positions[atoi(listIndices[0].c_str()) - 1];	//subtracting by 1 because the indices in the obj file start from 1 (and not 0)
					vert.m_color = Rgba8::WHITE;
					if (listIndices.size() > 1 && listIndices[1] != "")
					{
						vert.m_uvTexCoords = uvs[atoi(listIndices[1].c_str()) - 1];
					}
					if (listIndices.size() > 2)
					{
						vert.m_normal = normals[atoi(listIndices[2].c_str()) - 1];
					}
					
					m_vertices.push_back(vert);
				}
			}

			//add indices for index buffer
			if (numberOfClusters > 3)
			{
				//if there were 4 clusters, mean it was a quad
				m_indices.push_back(baseIndex + 0);
				m_indices.push_back(baseIndex + 1);
				m_indices.push_back(baseIndex + 2);
				m_indices.push_back(baseIndex + 0);
				m_indices.push_back(baseIndex + 2);
				m_indices.push_back(baseIndex + 3);
			}
			else
			{
				//only 3 clusters, means it was a triangle
				m_indices.push_back(baseIndex + 0);
				m_indices.push_back(baseIndex + 1);
				m_indices.push_back(baseIndex + 2);
			}
		}
	}

	Mat44 transformMatrix;
	transformMatrix.Append(Mat44::CreateUniformScale3D(importOptions.m_scale));
	transformMatrix.Append(importOptions.m_transform);
	TransformVertexArray3D((int)m_vertices.size(), m_vertices.data(), transformMatrix);

	if (importOptions.m_reverseWindingOrder)
	{
		ReverseWindingOrder();
	}
	if (importOptions.m_invertV)
	{
		InvertV();
	}

	m_isCPUMeshDirty = true;
	return true;
}

unsigned int MeshBuilder::GetNumVertices() const
{
	return (unsigned int)m_vertices.size();
}

const std::vector<Vertex_PNCU> MeshBuilder::GetVerticesData() const
{
	return m_vertices;
}

size_t MeshBuilder::GetSizeOfVertex() const
{
	return sizeof(m_vertices[0]);
}

unsigned int MeshBuilder::GetNumIndices() const
{
	return (unsigned int)m_indices.size();
}

const std::vector<unsigned int> MeshBuilder::GetIndicesData() const
{
	return m_indices;
}

size_t MeshBuilder::GetSizeOfIndex() const
{
	return sizeof(m_indices[0]);
}

bool MeshBuilder::IsCPUMeshDirty() const
{
	return m_isCPUMeshDirty;
}

void MeshBuilder::SetCPUMeshDirty(bool dirty)
{
	m_isCPUMeshDirty = dirty;
}

void MeshBuilder::TransformVerts(const Mat44& transform)
{
	//Mat44 invTransform = transform.GetOrthonormalInverse();
	TransformVertexArray3D((int)m_vertices.size(), m_vertices.data(), transform);
	m_isCPUMeshDirty = true;
}

void MeshBuilder::ReverseWindingOrder()
{
	//swap the 2nd and 3rd indices for each set of 3 indices
	for (int i = 0; i < m_indices.size(); i += 3)
	{
		int temp = m_indices[i + 1];
		m_indices[i + 1] = m_indices[i + 2];
		m_indices[i + 2] = temp;
	}
	m_isCPUMeshDirty = true;
}

void MeshBuilder::InvertV()
{
	for (int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].m_uvTexCoords.y = 1.f - m_vertices[i].m_uvTexCoords.y;
	}
	m_isCPUMeshDirty = true;
}

void MeshBuilder::ApplyScale(float scale)
{
	Mat44 scaleMatrix = Mat44::CreateUniformScale3D(scale);
	TransformVertexArray3D((int)m_vertices.size(), m_vertices.data(), scaleMatrix);
	m_isCPUMeshDirty = true;
}


void MeshBuilder::Save(const char* filepath)
{
	FileStream fileStream;
	fileStream.OpenForWrite(filepath);
	std::string numVerts = std::to_string(GetNumVertices());
	fileStream.WriteBytes(numVerts.c_str(), sizeof(unsigned int));

	size_t vertexDataSize = sizeof(m_vertices[0]) * GetNumVertices();
	char* vertexData = new char[vertexDataSize];
	memcpy(vertexData, m_vertices.data(), vertexDataSize);
	fileStream.WriteBytes(vertexData, vertexDataSize);
	delete[] vertexData;

	std::string numIndices = std::to_string(GetNumIndices());
	fileStream.WriteBytes(numIndices.c_str(), sizeof(unsigned int));

	size_t indexDataSize = sizeof(m_indices[0]) * GetNumIndices();
	char* indexData = new char[indexDataSize];
	memcpy(indexData, m_indices.data(), indexDataSize);
	fileStream.WriteBytes(indexData, indexDataSize);
	delete[] indexData;

	fileStream.Close();
}

void MeshBuilder::Load(const char* filepath)
{
	FileStream fileStream;
	fileStream.OpenForRead(filepath);
	char* vertexCount = new char[sizeof(unsigned int)];
	fileStream.ReadBytes(vertexCount, sizeof(unsigned int));
	unsigned int verts = strtoul(vertexCount, nullptr, 0);
	delete[] vertexCount;

	size_t vertexDataSize = sizeof(Vertex_PNCU) * verts;
	m_vertices.resize(verts);
	char* vertexData = new char[vertexDataSize];
	fileStream.ReadBytes(vertexData, vertexDataSize);
	memcpy(m_vertices.data(), vertexData, vertexDataSize);
	delete[] vertexData;

	char* indexCount = new char[sizeof(unsigned int)];
	fileStream.ReadBytes(indexCount, sizeof(unsigned int));
	unsigned int indices = strtoul(indexCount, nullptr, 0);
	delete[] indexCount;

	size_t indexDataSize = sizeof(unsigned int) * indices;
	m_indices.resize(indices);
	char* indexData = new char[indexDataSize];
	fileStream.ReadBytes(indexData, indexDataSize);
	memcpy(m_indices.data(), indexData, indexDataSize);
	delete[] indexData;

	fileStream.Close();
	m_isCPUMeshDirty = true;
}

std::string MeshBuilder::MakeValuesStringFromTokens(const Strings& tokens)
{
	std::string valueString;
	//always ignore the first token, since it's the value type identifier token
	for (int i = 1; i < tokens.size(); i++)
	{
		//if token is non empty
		if (tokens[i].size() > 0)
		{
			valueString.append(tokens[i]);
			if (i != tokens.size() - 1)
				valueString.append(",");
		}
	}

	return valueString;
}
