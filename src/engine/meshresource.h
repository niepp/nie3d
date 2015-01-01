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
	char name[MAX_NAME_LEN];
};

struct ENGINE_API Mesh{
	IVertexBuffer *m_vb;
	IIndexBuffer *m_ib;
	IInputLayout *m_layout;
	Uint32 vertex_count;
	Uint32 index_count;
};

typedef std::vector<Mesh> MeshVec;

class ENGINE_API MeshResource : public Resource
{
public:
	MeshResource(const Filepath &file_path);
	Bool Load();
	void GetMeshData(std::vector<MeshData*> &pMeshes) { pMeshes = m_pMeshes; }
	MeshVec& GetMeshes() { return m_meshes; }
	static IInputLayout* GetMeshVertexLayout() { return m_meshVertexLayout; }
private:
	void ReadMesh(const char *meshFile, std::vector<MeshData*> &pMeshes);
private:
	MeshVec m_meshes;
	std::vector<MeshData*> m_pMeshes;
	static IInputLayout *m_meshVertexLayout;

};

//////////////////////////////////////////////////////////////////////////
template<typename T>
void inline PutData(std::vector<Uint8> &pbuf, Uint32 &size, const T &value)
{
	if (pbuf.size() < size + sizeof(T)) {
		pbuf.resize((size + sizeof(T)) * 2);
	}

	Uint8 *ptr = &pbuf[0];
	*(T*)(ptr + size) = value;
	size += sizeof(T);
}

template<typename T, Uint32 n>
void inline PutDataArray(std::vector<Uint8> &pbuf, Uint32 &size, const T *array)
{
	Uint32 len = n * sizeof(T);
	if (pbuf.size() < size + len + 4) {
		pbuf.resize((size + len + 4) * 2);
	}

	Uint8 *ptr = &pbuf[0];
	*(Uint32*)(ptr + size) = n;
	memcpy(ptr + size + sizeof(Uint32), array, len);
	size += len + sizeof(Uint32);
}

template<typename T>
void inline GetData(const Uint8 *(&pbuf), T &value)
{
	value = *(T*)(pbuf);
	pbuf += sizeof(T);
}

template<typename T>
void inline GetDataArray(const Uint8 *(&pbuf), T *array, Uint32 n)
{
	memcpy(array, pbuf, n * sizeof(T));
	pbuf += n * sizeof(T);
}

void ENGINE_API BuildMesh(std::vector<Uint8> &pbuf, const std::vector<MeshData*> &pMeshes);

void ENGINE_API ParseMesh(const Uint8 *pbuf, std::vector<MeshData*> &pMeshes);
