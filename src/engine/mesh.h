#pragma once

#include <vector>

struct ENGINE_API MeshVertex{
	Vector3 position;
	Vector3 normal;
	Vector4 tangent;
	Color color;
	Vector2 uv0;
	Vector2 uv1;
};

struct ENGINE_API MeshData{
	std::vector<MeshVertex> vertex_data;
	std::vector<Uint16> index_data;
	Uint32 uv_channel;
};

struct ENGINE_API Mesh{
	IVertexBuffer *m_vb;
	IIndexBuffer *m_ib;
	IInputLayout *m_layout;
	Uint32 vertex_count;
	Uint32 index_count;
};

typedef std::vector<Mesh> MeshVec;