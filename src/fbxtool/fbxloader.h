#pragma once

#include "fbxsdk.h"
#include <vector>

class FbxLoader {
private:
	FbxLoader();
	~FbxLoader();
	void VisitNode(FbxNode *pNode, std::vector<MeshData*> &pMeshes);
	bool ConvertMesh(FbxMesh *pMesh, MeshData *meshdata);
	void CalcTangent(MeshData *meshdata);
public:
	static FbxLoader& GetInstance();
	void LoadFbx(const char *fbxFile, std::vector<MeshData*> &pMeshes);

private:
	FbxManager *m_pManager;
	FbxScene *m_pScene;
	FbxGeometryConverter *m_pGeometryConverter;

};
