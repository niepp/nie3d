#include "core/core.h"

#include "engine.h"
#include "resource.h"
#include "meshresource.h"

IInputLayout *MeshResource::m_meshVertexLayout = NULL;

MeshResource::MeshResource(const Filepath &file_path) : Resource(file_path, cResTypeMesh)
{

}

Bool MeshResource::Load()
{
	if (m_meshVertexLayout == NULL) {
		InputElement elements0[] =
		{
			{0, 0, cInputUsagePosition, 0, cPixelFormatR32G32B32_Float},
			{0, 12, cInputUsageNormal,  0, cPixelFormatR32G32B32_Float},
			{0, 24, cInputUsageTangent, 0, cPixelFormatR32G32B32A32_Float},
			{0, 40, cInputUsageColor,   0, cPixelFormatR8G8B8A8_Unorm},
			{0, 44, cInputUsageTexcoord,0, cPixelFormatR32G32_Float},
			{0, 52, cInputUsageTexcoord,1, cPixelFormatR32G32_Float},
		};
		m_meshVertexLayout = GetRenderer()->CreateInputLayout(elements0, ArraySize(elements0));
	}

	const char *filename = m_file_path.AsCharPtr();
	m_pMeshes.clear();
	ReadMesh(filename, m_pMeshes);

	IRenderer *renderer = GetRenderer();
	std::vector<MeshData*>::iterator itr = m_pMeshes.begin();
	for (; itr != m_pMeshes.end(); ++itr) {
		Mesh mesh;
		mesh.vertex_count = (*itr)->vertex_data.size();
		mesh.index_count = (*itr)->index_data.size();
		mesh.m_vb = renderer->CreateVertexBuffer(cUsageStatic, (*itr)->vertex_data.size() * sizeof(MeshVertex), sizeof(MeshVertex));
		mesh.m_ib = renderer->CreateIndexBuffer(cUsageStatic, (*itr)->index_data.size(), cIndex16);
		mesh.m_vb->Update(&(*itr)->vertex_data[0], (*itr)->vertex_data.size() * sizeof(MeshVertex));
		mesh.m_ib->Update(&(*itr)->index_data[0], (*itr)->index_data.size() * sizeof(Uint16));
		mesh.m_layout = m_meshVertexLayout;
		m_meshes.push_back(mesh);
	}

	return true;

}

void MeshResource::ReadMesh(const char *meshFile, std::vector<MeshData*> &pMeshes)
{
	Uint32 size = 0;
	const Uint8 *pbuf = FileUtils::Instance()->ReadBinaryFile(meshFile, &size);
	if (pbuf) {
		ParseMesh(pbuf, pMeshes);
	}
}

//////////////////////////////////////////////////////////////////////////
void BuildMesh(std::vector<Uint8> &pbuf, const std::vector<MeshData*> &pMeshes)
{
	pbuf.clear();
	Uint32 size = 0;

	PutData<Uint32>(pbuf, size, pMeshes.size());

	std::vector<MeshData*>::const_iterator itr = pMeshes.begin();
	for (; itr != pMeshes.end(); ++itr)
	{
		MeshData *mdata = (*itr);
		PutDataArray<char, MAX_NAME_LEN>(pbuf, size, mdata->name);
		
		PutData<Uint32>(pbuf, size, mdata->vertex_data.size());
		for (std::vector<MeshVertex>::iterator i = mdata->vertex_data.begin();
			i != mdata->vertex_data.end(); ++i)
		{
			PutData<Vector3>(pbuf, size, i->position);
			PutData<Vector3>(pbuf, size, i->normal);
			PutData<Vector4>(pbuf, size, i->tangent);
			PutData<Color>(pbuf,   size, i->color);
			PutData<Vector2>(pbuf, size, i->uv0);
			PutData<Vector2>(pbuf, size, i->uv1);
		}

		PutData<Uint32>(pbuf, size, mdata->index_data.size());
		for (std::vector<Uint16>::iterator i = mdata->index_data.begin();
			i != mdata->index_data.end(); ++i)
		{
			PutData<Uint16>(pbuf, size, *i);
		}
	}
}

void ParseMesh(const Uint8 *pbuf, std::vector<MeshData*> &pMeshes)
{
	Uint32 count = 0;
	GetData<Uint32>(pbuf, count);
	for (Uint32 i = 0; i < count; ++i)
	{
		MeshData *mdata = new MeshData();
		Uint32 len = 0;
		GetData<Uint32>(pbuf, len);
		GetDataArray<char>(pbuf, mdata->name, len);

		Uint32 v_cnt = 0;
		GetData<Uint32>(pbuf, v_cnt);
		for (Uint32 j = 0; j < v_cnt; ++j)
		{
			MeshVertex vtx;
			GetData<Vector3>(pbuf, vtx.position);
			GetData<Vector3>(pbuf, vtx.normal);
			GetData<Vector4>(pbuf, vtx.tangent);
			GetData<Color>(pbuf,   vtx.color);
			GetData<Vector2>(pbuf, vtx.uv0);
			GetData<Vector2>(pbuf, vtx.uv1);
			mdata->vertex_data.push_back(vtx);
		}

		Uint32 i_cnt = 0;
		GetData<Uint32>(pbuf, i_cnt);
		for (Uint32 k = 0; k < i_cnt; ++k)
		{
			Uint16 idx = 0;
			GetData<Uint16>(pbuf, idx);
			mdata->index_data.push_back(idx);
		}
		pMeshes.push_back(mdata);
	}

}

