#pragma once

class ENGINE_API ResourceCache
{
	typedef std::map<String, Resource*> ResourceDictionary;
public:
	ResourceCache()
	{
	}

	virtual ~ResourceCache()
	{
	}

	static ResourceCache* Instance();

	Resource* FindResource(const String &resFilePath);
	void AddResource(const String &resFilePath, Resource *res);
	Resource* Create(const String &resFilePath, ResourceType resType);

protected:
	ResourceDictionary m_res_dict;
};
