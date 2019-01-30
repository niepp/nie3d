#include "core/core.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "resourcecache.h"
#include "material.h"

Material::Material(const Filepath &file_path) : Resource(file_path, cResTypeMaterial)
	, m_pCoreMtl(NULL), m_shader(NULL)
{
	m_mtlmacro.Reset();
}

Material::~Material()
{
}

Bool Material::Load()
{
	XmlDocument xmlDoc;
	Bool ret = XmlUtils::LoadXmlFromFile(&xmlDoc, m_file_path.AsCharPtr());
	if (!ret) {
		return false;
	}

	XmlNode *root = XmlUtils::FirstChildNode(&xmlDoc);
	if (XmlUtils::GetNodeName(root) == "material")
	{
		// shader
		XmlNode *shader_node = XmlUtils::FirstChildNode(root, "shader");
		XmlUtils::GetAttribute<String>(shader_node, "name", m_pShaderName);

		// technique
		XmlNode *tech_node = XmlUtils::FirstChildNode(root, "technique");
		XmlUtils::GetAttribute<String>(tech_node, "name", m_techName);

		// macro
		XmlNode *macro_node = XmlUtils::FirstChildNode(root, "shadermacro");
		int count = 0;
		XmlUtils::GetAttribute<int>(macro_node, "count", count);

		for (int i = 0; i < count; ++i)
		{
			String node_name;
			node_name.FormatInput("macro_%d", i);
			XmlNode *macro_node_i = XmlUtils::FirstChildNode(macro_node, node_name.AsCharPtr());
			String macroName;
			Uint32 macroValue;
			XmlUtils::GetAttribute<String>(macro_node_i, "name", macroName);
			XmlUtils::GetAttribute<Uint32>(macro_node_i, "value", macroValue);
			m_mtlmacro.SetMacro(macroName, macroValue);
		}

		// parameter
		XmlNode *param_node = XmlUtils::FirstChildNode(root, "parameter");
		count = 0;
		XmlUtils::GetAttribute<int>(param_node, "count", count);
		for (int i = 0; i < count; ++i)
		{
			String node_name;
			node_name.FormatInput("param_%d", i);
			XmlNode *param_node_i = XmlUtils::FirstChildNode(param_node, node_name.AsCharPtr());
			ReadParam(param_node_i);
		}

		// renderstate
		XmlNode *state_node = XmlUtils::FirstChildNode(root, "renderstate");
		count = 0;
		XmlUtils::GetAttribute<int>(state_node, "count", count);
		for (int i = 0; i < count; ++i)
		{
			String node_name;
			node_name.FormatInput("state_%d", i);
			XmlNode *state_node_i = XmlUtils::FirstChildNode(state_node, node_name.AsCharPtr());
			String stateName;
			Uint32 stateValue;
			XmlUtils::GetAttribute<String>(state_node_i, "name", stateName);
			XmlUtils::GetAttribute<Uint32>(state_node_i, "value", stateValue);
			RenderStateType type = RenderState::NameToType(stateName.AsCharPtr());
			m_mtlRenderState.SetRenderState(type, stateValue);
		}
	}

	InitShader(m_pShaderName.AsCharPtr());

	return true;
}

void Material::ReadParam(XmlNode *param_node)
{
	String paramName;
	String paramType;
	String ValueStr;
	Variant paramValue;
	XmlUtils::GetAttribute<String>(param_node, "name", paramName);
	XmlUtils::GetAttribute<String>(param_node, "type", paramType);
	XmlUtils::GetAttribute<String>(param_node, "value", ValueStr);

	Bool b;
	Int32 i;
	Float f;
	Vector2 f2;
	Vector3 f3;
	Vector4 f4;
	Color c;
	String s;

	VariantType vType = Variant::NameToType(paramType.AsCharPtr());
	switch (vType)
	{
	case cVarString: // texture paramter
		{
		Uint32 address(cAddressWarp);
		Uint32 filter(cFilterBilinear);
		XmlUtils::GetAttribute<Uint32>(param_node, "address", address);
		XmlUtils::GetAttribute<Uint32>(param_node, "filter", filter);
		MtlSam sampler;
		sampler.pname = paramName;
		sampler.addrstate = (AddressState)address;;
		sampler.filterstate = (FilterState)filter;
		m_matsamplers.push_back(sampler);

		Texture *tex_res = (Texture*)ResourceCache::Instance()->Create(ValueStr, cResTypeTexture);

		paramValue.SetObjectPtr(tex_res);
		}
		break;
	case cVarBool:

		if (StringTo(ValueStr, b))
		{
			paramValue.SetBool(b);
		}
		break;
	case cVarInt:
		if (StringTo(ValueStr, i))
		{
			paramValue.SetInt(i);
		}
		break;
	case cVarFloat:
		if (StringTo(ValueStr, f))
		{
			paramValue.SetFloat(f);
		}
		break;
	case cVarFloat2:
		if (StringTo(ValueStr, f2))
		{
			paramValue.SetFloat2(f2);
		}
		break;
	case cVarFloat3:
		if (StringTo(ValueStr, f3))
		{
			paramValue.SetFloat3(f3);
		}
		break;
	case cVarFloat4:
		if (StringTo(ValueStr, f4))
		{
			paramValue.SetFloat4(f4);
		}
		break;
	case cVarColor:
		if (StringTo(ValueStr, c))
		{
			paramValue.SetColor(c);
		}
		break;
	default :
		break;
	}

	MtlParam param;
	param.pname = paramName;
	param.var = paramValue;
	m_ParamArray.push_back(param);
}

void Material::SaveMaterial()
{

}

void Material::InitShader(const char* pshadertepname)
{
	m_pShaderName = pshadertepname;
	m_pCoreMtl = GetRenderer()->CreateMaterialTemplate(pshadertepname);
}

IShader* Material::GetRenderShader()
{
	if (m_shader != NULL)
	{
		return m_shader;
	}

	m_shader = m_pCoreMtl->GetShader(m_mtlmacro, m_techName);

	return m_shader;

}

void Material::SetParamsToShader(IShader* pShader)
{
	for(Uint32 i = 0; i < m_ParamArray.size(); ++i)
	{
		MtlParam& params = m_ParamArray[i];
		Variant& var = params.var;
		VariantType type =  var.GetType();
		switch(type)
		{
		case cVarObject:
			{
				Texture* tex_res = dynamic_cast<Texture*>(var.GetObjectPtr());
				if (tex_res != NULL) {
					ITexture *tex = tex_res->GetTexture();
					if (tex && pShader->SetTexture(params.pname, tex))
					{
						FilterState filter = tex->GetFilterMode();
						AddressState address = tex->GetAddressMode();
						if (GetMtlSamplers(params.pname, filter, address))
						{
							tex->SetFilterMode(filter);
							tex->SetAddressMode(address);
						}
					}
				}
			}
			break;
		case cVarBool:
			{
				bool data = var.GetBool();
				pShader->SetConstant(params.pname, &data);
			}
			break;
		case cVarFloat:
			{
				float data = var.GetFloat();
				pShader->SetConstant(params.pname, &data);
			}
			break;
		case cVarFloat2:
			{
				const Vector2& data = var.GetFloat2();
				pShader->SetConstant(params.pname, &data);
			}
			break;
		case cVarFloat3:
			{
				const Vector3& data = var.GetFloat3();
				pShader->SetConstant(params.pname, &data);
			}
			break;
		case cVarFloat4:
			{
				const Vector4& data = var.GetFloat4();
				pShader->SetConstant(params.pname, &data);
			}
			break;

		case cVarMatrix:
			{
				const Matrix& data = var.GetMatrix();
				pShader->SetConstant(params.pname, &data);
			}
			break;
		}

	}

}

void Material::SetMtlParam(const Uint32 index, const Variant &var)
{
	if (index < m_ParamArray.size())
	{
		m_ParamArray[index].var = var;
	}
}

void Material::SetMtlParam(const String &name, const Variant &var)
{	

	Uint32 index = m_pCoreMtl->FindParamByName(name);
	if( index != 0xffffffff )
	{
		m_ParamArray[index].pname = name;
		m_ParamArray[index].var = var;
	}
}

Bool Material::GetMtlParam(const String &name, Variant &var)
{
	Uint32 index = m_pCoreMtl->FindParamByName(name);
	if( index != 0xffffffff )
	{
		var = m_ParamArray[index].var;
		return true;
	}
	return false;
}

Bool Material::GetMtlParam(const Uint32 index, Variant &var)
{
	if (index >= m_ParamArray.size())
		return false;

	if (m_ParamArray[index].var.IsValid() == false)
		return false;

	var = m_ParamArray[index].var;
	return true;
}

Uint32 Material::GetMtlParamCount() const
{
	return m_ParamArray.size();
}

Material *Material::Clone()
{
	Material *pmtl = new Material(m_file_path);
	pmtl->m_mtlmacro = m_mtlmacro;
    pmtl->m_ParamArray = m_ParamArray;
	return pmtl;
}

Bool Material::GetMtlParamIndexByName(const String &name, Uint32& index)
{
	for(size_t i=0;i<m_ParamArray.size();i++)
	{
		if(m_ParamArray[i].pname == name)
		{
			index = i;
			return true;
		}
	}
	return false;
}

Bool Material::GetMtlParamNameByIndex( const Uint32 index, String& name )
{
	if(index >= m_ParamArray.size())
		return false;

	m_ParamArray[index].pname = m_pCoreMtl->GetParamDesc(index)->m_name;
	name = m_ParamArray[index].pname;
	return true;
}

void Material::SetMtlSamplers(const String &name, FilterState filterstate, AddressState addrstate)
{
	Uint32 index;
	if(GetMtlSamplersIndexByName(name,index))
	{
		m_matsamplers[index].filterstate = filterstate;
		m_matsamplers[index].addrstate = addrstate;
	}
}

Bool Material::GetMtlSamplers(const String &name, FilterState& filterstate, AddressState& addrstate)
{
	Uint32 index;
	if (GetMtlSamplersIndexByName(name,index))
	{
		filterstate = m_matsamplers[index].filterstate;
		addrstate = m_matsamplers[index].addrstate;
		return true;
	}
	return false;
}

Bool Material::GetMtlSamplersIndexByName(const String &name, Uint32& index)
{
	for(Uint32 i = 0; i < m_matsamplers.size(); ++i)
	{
		if(m_matsamplers[i].pname == name)
		{
			index = i;
			return true;
		}
	}
	return false;
}

