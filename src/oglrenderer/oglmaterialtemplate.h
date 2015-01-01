#pragma once
#include <vector>
#include <set>
#include <map>

class OGLRenderer;
class MaterialTemplate : public IMaterialTemplate
{
	struct ShaderObj
	{
		IShader *pshader;
		String techName;
		ShaderMacro shadermacro;
	};

	struct IncludeDesc
	{
		const char *start;
		const char *end;
		String file;
	};

public:
	MaterialTemplate(const String &path,const String &name);
	virtual ~MaterialTemplate();

	virtual Uint32 GetParamCount();
	virtual Uint32 FindParamByName(const String &name);

	virtual Uint32 GetMacroCount();
	virtual Uint32 FindMacroByName(const String &name);

	virtual const ShaderParamDesc* GetParamDesc(Uint32 index) const;
	virtual const ShaderMacroDesc* GetMacroDesc(Uint32 index) const;

	virtual IShader* GetShader(const ShaderMacro& macro, const String& techName);

private:
	void Initialize();
	void Destroy();

	void LoadTechniques(XmlNode *pnode);
	void LoadMacros(XmlNode *pnode);
	void LoadParams(XmlNode *pnode);

	Bool LoadFromXMLBuffer(const char *pbuf, Uint32 buflen);

	void ResolveInclude(const char *pbuf, Uint32 buflen, const char **outcode, Uint32 *codelen);

	TechniqueDesc* GetTechniqueDesc(const String& techName);

	IShader* CreateShader(const ShaderMacro& macro,const char *pvsEntry, const char *pfsEntry);

private:
	String m_path;
	String m_shader_name;

	const char* m_shader_code;
	Uint32 m_shader_code_size;

	std::vector<ShaderParamDesc*> m_params;
	std::vector<ShaderMacroDesc*> m_macros;

	std::vector<TechniqueDesc> m_technique_descs;
	std::vector<ShaderObj>m_shaders;

};


