#include <iostream>

#include "core/core.h"

#include "engine/engine.h"
#include "engine/resource.h"
#include "engine/meshresource.h"
#include "fbxloader.h"

bool ConvertMesh(const char *fbxFile, const char *meshFile)
{
	std::vector<Uint8> pbuf;
	std::vector<MeshData*> pMeshes;
	FbxLoader::GetInstance().LoadFbx(fbxFile, pMeshes);
	if (pMeshes.size() == 0) {
		return false;
	}

	BuildMesh(pbuf, pMeshes);

	FILE *fp = ::fopen(meshFile, "wb+");
	if (!fp) {
		return false;
	}

	fwrite(&pbuf[0], sizeof(Uint8), pbuf.size(), fp);
	fclose(fp);
	return true;
}

bool ConvertAnimation(const char *fbxFile, const char *meshFile)
{
	return true;
}

const char* GetApkPath()
{
	String path("hello-world!");
	std::cout<<path.AsCharPtr()<<std::endl;
	return path.AsCharPtr();
}


int main(int argc, char* argv[])
{
//	{
//		Sphere s(50);
//		std::vector<MeshData*> pMeshes;
//		MeshData *md = new MeshData;
//		pMeshes.push_back(md);
//		strcpy(md->name, "sphere");
//		md->index_data = s.m_sphere_indices;
//		for (int i = 0; i < s.m_sphere_pos.size(); ++i)
//		{
//			MeshVertex mv;
//			mv.position = s.m_sphere_pos[i];
//			mv.uv0 = s.m_sphere_uv[i];
//			mv.uv1.Set(0,0);
//			mv.normal = s.m_sphere_nor[i];
//			mv.tangent.Set(s.m_sphere_pos[i].y, s.m_sphere_pos[i].x, 0, 0);
//			mv.color = Color::White;
//			md->vertex_data.push_back(mv);
//		}
//		std::vector<Uint8> pbuf;	
//		BuildMesh(pbuf, pMeshes);
//		const char *meshFile = "E:/age/res/sphere.mesh";
//		FILE *fp = ::fopen(meshFile, "wb+");
//		fwrite(&pbuf[0], sizeof(Uint8), pbuf.size(), fp);
//		fclose(fp);
//return 0;
//	}

	if (argc != 4) {
		std::cout<<"Usage: -m -fbxpath -meshpath "<<std::endl;
		return 0;
	}

	if (argv[1][1] == 'm')
	{
		ConvertMesh(argv[2], argv[3]);
	}
	else if (argv[1][1] == 'a')
	{
		ConvertAnimation(argv[2], argv[3]);
	}

	return 0;
}

