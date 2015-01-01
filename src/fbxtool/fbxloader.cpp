#include "core/core.h"
#include "engine/engine.h"
#include "engine/resource.h"
#include "engine/meshresource.h"
#include "fbxloader.h"
#include <algorithm>
#include <iostream>

#ifdef _DEBUG
#pragma comment(lib, "../../../thirdparty/fbx/lib/vs2008/x86/debug/libfbxsdk.lib")
#else
#pragma comment(lib, "../../../thirdparty/fbx/lib/vs2008/x86/release/libfbxsdk.lib")
#endif

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

Vector2 FbxToVector(FbxVector2 &fbxVec)
{
	return Vector2((Float)fbxVec[0], (Float)fbxVec[1]);
}

Vector3 FbxToVector(FbxVector4 &fbxVec)
{
	return Vector3((Float)fbxVec[0], (Float)fbxVec[1], (Float)fbxVec[2]);
}

Color FbxToColor(FbxColor &fbxColor)
{
	return Color((Uint8)(255.0f * (float)fbxColor.mRed),
				(Uint8)(255.0f * (float)fbxColor.mGreen),
				(Uint8)(255.0f * (float)fbxColor.mBlue),
				(Uint8)(255.0f * (float)fbxColor.mAlpha));
}

void InitializeSdkObjects(FbxManager*& pManager)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	pManager = FbxManager::Create();
	if( !pManager )
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

}

void DestroySdkObjects(FbxManager* pManager)
{
	//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
	if(pManager) {
		pManager->Destroy();
		pManager = NULL;
	}
}

bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor,  lSDKMinor,  lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(pManager,"");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if( !lImportStatus )
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		FBXSDK_printf("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");

		for(i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			FBXSDK_printf("    Animation Stack %d\n", i);
			FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);

	if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		FBXSDK_printf("Please enter password: ");

		lPassword[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

		IOS_REF.SetStringProp(IMP_FBX_PASSWORD,      lString);
		IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		lStatus = lImporter->Import(pScene);

		if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("\nPassword is wrong, import aborted.\n");
		}
	}

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}


FbxLoader& FbxLoader::GetInstance()
{
	static FbxLoader fbxloader;
	return fbxloader;
}

FbxLoader::FbxLoader()
{
	InitializeSdkObjects(m_pManager);
	
	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	m_pScene = FbxScene::Create(m_pManager, "Scene");
	if(m_pScene == NULL) {
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
	
	FbxAxisSystem maxAxis(FbxAxisSystem::eZAxis, FbxAxisSystem::eParityEven, FbxAxisSystem::eRightHanded);
	FbxSystemUnit meter(1.0f);
	m_pScene->GetGlobalSettings().SetAxisSystem(maxAxis);
	m_pScene->GetGlobalSettings().SetSystemUnit(meter);

	m_pGeometryConverter = new FbxGeometryConverter(m_pManager);
}

FbxLoader::~FbxLoader()
{
	//DestroySdkObjects(m_pManager);
	//if(m_pScene != NULL) {
	//	m_pScene->Destroy();
	//	m_pScene = NULL;
	//}

	//if (m_pGeometryConverter != NULL) {
	//	delete m_pGeometryConverter;
	//	m_pGeometryConverter = NULL;
	//}
}

void FbxLoader::LoadFbx(const char *fbxFile, std::vector<MeshData*> &pMeshes)
{
	LoadScene(m_pManager, m_pScene, fbxFile);
	FbxNode *pRootNode = m_pScene->GetRootNode();

	VisitNode(pRootNode, pMeshes);

}

void FbxLoader::VisitNode(FbxNode *pNode, std::vector<MeshData*> &pMeshes)
{
	assert(pNode);

	FbxNodeAttribute *pAttrib = pNode->GetNodeAttribute();
	if(pAttrib) {
		FbxNodeAttribute::EType type = pAttrib->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh) {
			FbxMesh *pMesh = pNode->GetMesh();
			MeshData *pMeshData = new MeshData;
			const char *str = pMesh->GetName();
			Uint32 len = strlen(str);
			if (len == 0) {
				str = "NoName";
			}
			
			strncpy(pMeshData->name, str, MAX_NAME_LEN - 1);
			pMeshData->name[MAX_NAME_LEN - 1] = '\0';
			ConvertMesh(pMesh, pMeshData);
			pMeshes.push_back(pMeshData);
		}
	}

	int childCount = pNode->GetChildCount();
	for(int i = 0; i < childCount; ++i)
	{
		VisitNode(pNode->GetChild(i), pMeshes);
	}

}

bool FbxLoader::ConvertMesh(FbxMesh *pMesh, MeshData *meshdata)
{
	//remove the bad polygons before getting any data from mesh
	pMesh->RemoveBadPolygons();

	int LayerSmoothingCount = pMesh->GetLayerCount(FbxLayerElement::eSmoothing);
	for(int i = 0; i < LayerSmoothingCount; ++i)
	{
		FbxLayerElementSmoothing *smoothing = pMesh->GetLayer(i, FbxLayerElement::eSmoothing)->GetSmoothing();
		if (smoothing->GetMappingMode() == FbxLayerElement::eByEdge) {
			m_pGeometryConverter->ComputePolygonSmoothingFromEdgeSmoothing(pMesh, i);
		}
	}

	if (!pMesh->IsTriangleMesh()) {
		pMesh = (FbxMesh*)m_pGeometryConverter->Triangulate(pMesh, true); // not in place ! the old mesh is still there
		if (pMesh == NULL) {
			assert(0);
			return false; // not clean, missing some dealloc
		}
	}

	//Get the base layer of the mesh
	FbxLayer *BaseLayer = pMesh->GetLayer(0);
	if (BaseLayer == NULL) {
		return false;
	}

	// UVSet Channels
	const int layerCount = pMesh->GetLayerCount();
	std::vector<const char*> UVSets;
	UVSets.clear();
	for (int i = 0; i < layerCount; ++i)
	{
		FbxLayer *layer = pMesh->GetLayer(i);
		int UVSetCount = layer->GetUVSetCount();
		if(UVSetCount) {
			FbxArray<FbxLayerElementUV const*> EleUVs = layer->GetUVSets();
			for (int j = 0; j < UVSetCount; ++j)
			{
				FbxLayerElementUV const* ElementUV = EleUVs[j];
				if (ElementUV) {
					UVSets.push_back(ElementUV->GetName());
				}
			}
		}
	}

	int ctrlPtCount = pMesh->GetControlPointsCount();
	FbxVector4 *ctrlPoints = pMesh->GetControlPoints();

	std::vector<MeshVertex> &vertices = meshdata->vertex_data;
	vertices.resize(ctrlPtCount);
	std::vector<bool> assigned(ctrlPtCount, false);

	for(int i = 0; i < ctrlPtCount; ++i) {
		vertices[i].position = FbxToVector(ctrlPoints[i]);
	}

	int triCount = pMesh->GetPolygonCount();

	// Vertex Color
	for(int i = 0; i < layerCount; ++i)
	{
		FbxLayer *layer = pMesh->GetLayer(i);
		if (layer) {
			const FbxLayerElementVertexColor *ElementColor = layer->GetVertexColors();
			if(ElementColor) {
				FbxLayerElement::EMappingMode mapMode = ElementColor->GetMappingMode();
				FbxLayerElement::EReferenceMode refMode = ElementColor->GetReferenceMode();
				FbxLayerElementArrayTemplate<FbxColor> &colorArray = ElementColor->GetDirectArray();
				if (mapMode == FbxLayerElement::eByPolygonVertex) {
					for(int i = 0; i < triCount; ++i) {
						for(int j = 0; j < 3; ++j) {
							int colorIndex = i * 3 + j;
							if(refMode == FbxLayerElement::eIndexToDirect) {
								colorIndex = ElementColor->GetIndexArray().GetAt(i * 3 + j);
							}
							
							int vIndex = pMesh->GetPolygonVertex(i, j);
							vertices[vIndex].color = FbxToColor(colorArray[colorIndex]);
						}
					}
				}
				else if(mapMode == FbxLayerElement::eByControlPoint) {
					for (int v = 0; v < ctrlPtCount; ++v) {
						int colorIndex = v;
						if(refMode == FbxLayerElement::eIndexToDirect) {
							colorIndex = ElementColor->GetIndexArray().GetAt(v);
						}
						vertices[v].color = FbxToColor(colorArray[colorIndex]);
					}
				}
			}
		}
	}

	std::vector<Uint16> &triIndices = meshdata->index_data;
	triIndices.resize(triCount * 3);
	int *fbxTriIndices = pMesh->GetPolygonVertices();
	for(int i = 0; i < triCount * 3; ++i) {
		triIndices[i] = fbxTriIndices[i];
	}

	Uint32 uvCount = UVSets.size();

	assert(uvCount <= 2);

	for(int i = 0; i < triCount; ++i) {
		for(int j = 0; j < 3; ++j) {
			FbxVector4 pNormal;
			pMesh->GetPolygonVertexNormal(i, j, pNormal);
			Vector3 triVertexNormal = FbxToVector(pNormal);
			int controlPointIndex = pMesh->GetPolygonVertex(i, j);
			Vector3 pos = FbxToVector(ctrlPoints[controlPointIndex]);
			Vector2 uv[2];
			for (Uint32 c = 0; c < uvCount; ++c) {
				FbxVector2 fbxUV(0, 0);
				bool unMap = false;
				pMesh->GetPolygonVertexUV(i, j, UVSets[c], fbxUV, unMap);
				if (!unMap) {
					uv[c] = FbxToVector(fbxUV);
				}
			}

			MeshVertex &vertex = vertices[controlPointIndex];
			if (!assigned[controlPointIndex]) {
				assigned[controlPointIndex] = true;
				vertex.normal = triVertexNormal;
				vertex.uv0 = uv[0];
				vertex.uv1 = uv[1];
			}
			else if (triVertexNormal != vertex.normal) {
				bool newNor = true;
				for(Uint32 k = ctrlPtCount; k < vertices.size(); ++k)
				{
					if (pos == vertices[k].position && 
						triVertexNormal == vertices[k].normal) {
						triIndices[i * 3 + j] = k;
						newNor = false;
						break;
					}
				}
				if (newNor) {
					triIndices[i * 3 + j] = vertices.size();
					MeshVertex v = vertex;
					v.normal = triVertexNormal;
					vertices.push_back(v);
				}
			}
		}
	}

	CalcTangent(meshdata);

	return true;

}

// 对三角形，根据法线，和u,v坐标计算顶点切线，构建顶点的切空间
void FbxLoader::CalcTangent(MeshData *meshdata)
{
	MeshVertex *vertexList = &meshdata->vertex_data[0];
	Uint16 *indexList = &meshdata->index_data[0];
	Uint32 v_count = meshdata->vertex_data.size();
	Uint32 tri_count = meshdata->index_data.size() / 3;
	
	Vector3 *tan1 = new Vector3[v_count * 2];
	Vector3 *tan2 = tan1 + v_count;
	::memset(tan1, 0, v_count * sizeof(Vector3) * 2);

	for (Uint32 i = 0; i < tri_count; ++i)
	{
		Uint32 i1 = indexList[3 * i + 0];
		Uint32 i2 = indexList[3 * i + 1];
		Uint32 i3 = indexList[3 * i + 2];

		// position
		const Vector3& v1 = vertexList[i1].position;
		const Vector3& v2 = vertexList[i2].position;
		const Vector3& v3 = vertexList[i3].position;

		// texture coordinate
		const Vector2& w1 = vertexList[i1].uv0;
		const Vector2& w2 = vertexList[i2].uv0;
		const Vector2& w3 = vertexList[i3].uv0;

		Float x1 = v2.x - v1.x;
		Float x2 = v3.x - v1.x;
		Float y1 = v2.y - v1.y;
		Float y2 = v3.y - v1.y;
		Float z1 = v2.z - v1.z;
		Float z2 = v3.z - v1.z;

		Float s1 = w2.x - w1.x;
		Float s2 = w3.x - w1.x;
		Float t1 = w2.y - w1.y;
		Float t2 = w3.y - w1.y;

	//	assert(s1 * t2 - s2 * t1 != 0);

		Float r = 1.0f / (s1 * t2 - s2 * t1);
		Vector3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		Vector3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;

	}

	for (Uint32 i = 0; i < v_count; ++i)
	{
		const Vector3& n = vertexList[i].normal;
		const Vector3& t = tan1[i];

		// Gram-Schmidt orthogonalize
		Vector3 q = (t - n * Dot(n, t));
		q.Normalize();
		
		// Calculate handedness
		Float w = (Dot(Cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;
		vertexList[i].tangent.Set(q.x, q.y, q.z, w);
	}

	delete[] tan1;
	tan1 = NULL;

}

