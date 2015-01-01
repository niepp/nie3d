#include "core/core.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"
#include "meshresource.h"
#include "resourcecache.h"

ResourceCache* ResourceCache::Instance()
{
	static ResourceCache sResourceCache;
	return &sResourceCache;
}

Resource* ResourceCache::FindResource(const String &resFilePath)
{
	ResourceDictionary::iterator itr = m_res_dict.find(resFilePath);
	if (itr != m_res_dict.end()) {
		return itr->second;
	}
	return NULL;
}

void ResourceCache::AddResource(const String &resFilePath, Resource *res)
{
	m_res_dict[resFilePath] = res;
}

Resource* ResourceCache::Create(const String &resFilePath, ResourceType resType)
{
	Resource *res = FindResource(resFilePath);
	if (res != NULL) {
		res->IncRefCount();
		return res;
	}

	switch(resType)
	{
	case cResTypeTexture:
		res = new Texture(resFilePath);
		break;
	case cResTypeMaterial:
		res = new Material(resFilePath);
		break;
	case cResTypeMesh:
		res = new MeshResource(resFilePath);
		break;
	default:
		break;
	}

	if (!res || !res->Load()) {
		LogUtils::Instance()->LogInfo("[%s] load failed!", resFilePath.AsCharPtr());
		return NULL;
	}

	ResourceCache::Instance()->AddResource(resFilePath, res);

	return res;

}

