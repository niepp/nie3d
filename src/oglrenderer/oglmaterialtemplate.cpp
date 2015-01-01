#include "core/core.h"

#include "ogl.h"
#include "ogltexture.h"
#include "oglshader.h"
#include "oglmaterialtemplate.h"

MaterialTemplate::MaterialTemplate(const String &path, const String &name) 
: m_path(path)
, m_shader_name(name)
, m_shader_code(NULL)
, m_shader_code_size(0)
{
	Initialize();
}

MaterialTemplate::~MaterialTemplate()
{
	Destroy();
}

void MaterialTemplate::Initialize()
{
	String filename = String(m_path.AsCharPtr()) + m_shader_name.AsCharPtr();
	Uint32 nSize = 0;
	Int8 *pMtlTxt = FileUtils::Instance()->ReadTextFile(filename.AsCharPtr(), &nSize);

	if (pMtlTxt == NULL) {
		return;
	}

	char *phead = strstr(pMtlTxt, "/*<Material>");
	if(phead == NULL) {
		return;
	}
	phead += 2;

	char *ptail = strstr(phead, "</Material>*/");
	if(ptail == NULL) {
		return;
	}

	const char *code = ptail + strlen("</Material>*/");
	ResolveInclude(code, nSize + 1 - (code - pMtlTxt), &m_shader_code, &m_shader_code_size);

	ptail += strlen("</Material>");

	if(!LoadFromXMLBuffer(phead, ptail - phead)) {
		return;
	}

	delete[](pMtlTxt);

}

void MaterialTemplate::Destroy()
{
	delete[] m_shader_code;
	m_shader_code = NULL;
	m_shader_code_size = 0;

	for(Uint32 i = 0; i < m_params.size(); ++i)
	{
		delete(m_params[i]);
	}
	m_params.clear();

	for(Uint32 i = 0; i < m_macros.size(); ++i)
	{
		delete(m_macros[i]);
	}
	m_macros.clear();

	for(Uint32 i = 0; i < m_shaders.size(); ++i)
	{
		delete(m_shaders[i].pshader);
	}
	m_shaders.clear();

	m_technique_descs.clear();

}

void MaterialTemplate::ResolveInclude(const char *pbuf, size_t buflen, const char **outcode, Uint32 *codelen)
{
	std::vector<IncludeDesc> include_files;

	const char *match0 = "#include \"";
	const char *match1 = "\"";
	Uint32 len0 = strlen(match0);
	Uint32 len1 = strlen(match1);

	const char *s = pbuf;
	const char *p = s;
	for (; p != NULL; )
	{
		IncludeDesc desc;
		p = strstr(s, match0);
		if (p == NULL) {
			break;
		}

		desc.start = p;
		p += len0;

		const char *q = strstr(p, match1);
		if (q == NULL) {
			break;
		}

		desc.end = q;
		char headfile[MAX_PATH_LEN] = {0};
		strncpy(headfile, p, q - p);
		desc.file = headfile;
		include_files.push_back(desc);

		q += len1;
		s = q;

	}

	String code("");
	const char *now = pbuf;
	FILE *fp = NULL;
	std::vector<IncludeDesc>::iterator itr = include_files.begin();
	for (; itr != include_files.end(); ++itr)
	{
		String headfile = m_path.AsCharPtr() + itr->file;
		Uint32 nSize = 0;
		Int8 *fxstr = FileUtils::Instance()->ReadTextFile(headfile.AsCharPtr(), &nSize);
		if (fxstr == NULL)	{
			printf("%s not found!\n", headfile.AsCharPtr());
			continue;
		}

		const char *headcode = NULL;
		Uint32 headlen = 0;
		ResolveInclude(fxstr, nSize + 1, &headcode, &headlen);

		size_t seglen = itr->start - now;
		String seg(now, seglen);

#if PLATFORM == PLATFORM_WIN32
		code += seg + String("\n");
		code += headcode + String("\n");
#else
		code += seg + String("\r\n");
		code += headcode + String("\r\n");
#endif
		now = itr->end + 1;

		delete[] headcode;
		headcode = NULL;

		delete[] fxstr;
		fxstr = NULL;

	}

	String tail(pbuf);
	if (now - pbuf > 0) {
		tail.SubStringToSelf(now - pbuf);
	}

	code += tail;

	*codelen = code.GetLength();
	char *str = new char[*codelen + 1];
	strcpy(str, code.AsCharPtr());
	str[*codelen] = '\0';
	*outcode = str;

}

Bool MaterialTemplate::LoadFromXMLBuffer(const char *pbuf, Uint32 buflen)
{
	XmlDocument xmlDoc;
	String str = pbuf;
	str.SubStringToSelf(0, buflen);
	XmlUtils::LoadXmlFromString(&xmlDoc, str.AsCharPtr());

	XmlNode *proot = XmlUtils::FirstChildNode(&xmlDoc);

	LoadTechniques(proot);
	LoadMacros(XmlUtils::FirstChildNode(proot, "Macros"));
	LoadParams(XmlUtils::FirstChildNode(proot, "Params"));

	return true;
}

void MaterialTemplate::LoadTechniques(XmlNode *pnode)
{
	XmlNode* pchild = XmlUtils::FirstChildNode(pnode, "technique");
	for (; pchild; pchild = XmlUtils::NextSiblingNode(pchild,"technique"))
	{
		TechniqueDesc techDesc;
		XmlUtils::GetAttribute<String>(pchild, "name", techDesc.tech_name);
		XmlUtils::GetAttribute<String>(pchild, "vsEntry", techDesc.vs_entry);
		XmlUtils::GetAttribute<String>(pchild, "psEntry", techDesc.ps_entry);
		m_technique_descs.push_back(techDesc);
	}
}

void MaterialTemplate::LoadMacros(XmlNode *pnode)
{
	if (pnode == NULL)
	{
		return;
	}

	for (XmlNode* pchild = XmlUtils::FirstChildNode(pnode); pchild; pchild = XmlUtils::NextSiblingNode(pchild))
	{
		ShaderMacroDesc *pvar = new ShaderMacroDesc;
		XmlUtils::GetAttribute<String>(pchild, "name", pvar->m_name);

		if(XmlUtils::HasAttributeValue(pchild, "UIName"))
		{
			XmlUtils::GetAttribute<String>(pchild, "UIName", pvar->m_ui_name);
		}
		else {
			pvar->m_ui_name = pvar->m_name;
		}

		pvar->m_macro = true;
		if(XmlUtils::HasAttributeValue(pchild, "default"))
		{
			XmlUtils::GetAttribute<Bool>(pchild, "default", pvar->m_macro);
		}
		
		pvar->m_display = true;
		if(XmlUtils::HasAttributeValue(pchild, "display"))
		{
			XmlUtils::GetAttribute<Bool>(pchild, "display", pvar->m_display);
		}

		m_macros.push_back(pvar);
	}
}

void MaterialTemplate::LoadParams(XmlNode *pnode)
{
	if (pnode == NULL)
	{
		return;
	}

	for (XmlNode* pchild = XmlUtils::FirstChildNode(pnode); pchild; pchild = XmlUtils::NextSiblingNode(pchild))
	{
		ShaderParamDesc *pvar = new ShaderParamDesc;

		XmlUtils::GetAttribute<String>(pchild, "name", pvar->m_name);
		VariantType vartype;
		String name;
		XmlUtils::GetAttribute<String>(pchild, "type", name);
		vartype = Variant::NameToType(name.AsCharPtr());

		if(XmlUtils::HasAttributeValue(pchild, "UIType"))
		{
			XmlUtils::GetAttribute<String>(pchild, "UIType", pvar->m_ui_type);
		}
		else
		{
			pvar->m_ui_type = "";
		}

		if(XmlUtils::HasAttributeValue(pchild, "UIName"))
		{
			XmlUtils::GetAttribute<String>(pchild, "UIName", pvar->m_ui_name);
		}
		else
		{
			pvar->m_ui_name = pvar->m_name;
		}

		if(XmlUtils::HasAttributeValue(pchild, "default"))
		{
			String s;
			XmlUtils::GetAttribute<String>(pchild, "default", s);
			pvar->m_def_value.Set(vartype, s.AsCharPtr());
		}

		if(XmlUtils::HasAttributeValue(pchild, "min"))
		{
			String s;
			XmlUtils::GetAttribute<String>(pchild, "min", s);
			pvar->m_min_value.Set(vartype, s.AsCharPtr());
		}

		if(XmlUtils::HasAttributeValue(pchild, "max"))
		{
			String s;
			XmlUtils::GetAttribute<String>(pchild, "max", s);
			pvar->m_max_value.Set(vartype, s.AsCharPtr());
		}

		pvar->m_display = true;
		if(XmlUtils::HasAttributeValue(pchild, "display"))
		{
			XmlUtils::GetAttribute<Bool>(pchild, "display", pvar->m_display);
		}
		m_params.push_back(pvar);

	}

}

Uint32 MaterialTemplate::GetParamCount()
{
	return m_params.size();
}

Uint32 MaterialTemplate::FindParamByName(const String &name)
{
	for(Uint32 i = 0; i < m_params.size(); ++i)
	{
		if(m_params[i]->m_name == name)
			return i;
	}
	return cInvalidIndex;
}

Uint32 MaterialTemplate::GetMacroCount()
{
	return m_macros.size();
}

Uint32 MaterialTemplate::FindMacroByName(const String &name)
{
	for(Uint32 i = 0; i < m_macros.size(); ++i)
	{
		if(m_macros[i]->m_name == name)
			return i;
	}
	return cInvalidIndex;
}

const MaterialTemplate::ShaderParamDesc* MaterialTemplate::GetParamDesc(Uint32 index) const
{
	assert(index <= m_params.size());
	return m_params[index];
}

const MaterialTemplate::ShaderMacroDesc* MaterialTemplate::GetMacroDesc(Uint32 index) const
{
	assert(index <= m_macros.size());
	return m_macros[index];
}

IShader* MaterialTemplate::GetShader(const ShaderMacro& macro, const String& techName)
{
	for(Uint32 i = 0; i < m_shaders.size(); ++i)
	{
		ShaderObj &obj = m_shaders[i];
		if(obj.shadermacro == macro && obj.techName == techName)
		{
			return obj.pshader;
		}
	}

	TechniqueDesc* ptechDesc = GetTechniqueDesc(techName);

	ShaderObj obj;
	if(ptechDesc == NULL)
	{
		return NULL;
	}
	else
	{
		obj.shadermacro = macro;
		obj.techName = techName;
		obj.pshader = CreateShader(macro, ptechDesc->vs_entry.AsCharPtr(), ptechDesc->ps_entry.AsCharPtr());
		m_shaders.push_back(obj);
		return obj.pshader;
	}
}

IShader *MaterialTemplate::CreateShader(const ShaderMacro& macro, const char* pvsEntry, const char* pfsEntry)
{
	OGLShader *pshader = new OGLShader();
	const Uint32 macro_count = ShaderMacro::macro_count;
	char* macrobuf[macro_count];
	char buf[macro_count][256];
	memset(buf, 0, sizeof(buf));
	for (Uint32 i = 0; i < macro_count; ++i)
	{
		macrobuf[i] = buf[i];
	}
	Uint32 count = 0;
	macro.GetDefineStr(macrobuf, count);
	pshader->BuildShader(m_shader_code, m_shader_code_size, (const char **)macrobuf, count, pvsEntry, pfsEntry);

	return pshader;

}

TechniqueDesc* MaterialTemplate::GetTechniqueDesc(const String &techName)
{
	for(Uint32 i = 0; i < m_technique_descs.size(); ++i)
	{
		if( m_technique_descs[i].tech_name == techName)
			return &m_technique_descs[i];
	}
	return NULL;

}