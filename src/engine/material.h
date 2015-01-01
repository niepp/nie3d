#pragma once
#include <vector>
#include <map>

class Texture;
class ENGINE_API Material : public Resource
{
	struct MtlParam
	{
		String pname;
		Variant var;
	};

	struct MtlSam
	{
		String pname;
		FilterState filterstate;
		AddressState addrstate;
	};

public:
	Material(const Filepath &file_path);
	virtual ~Material();

	virtual Bool Load();
	void SaveMaterial();
	void InitShader(const char* pshadertepname);
	IMaterialTemplate* GetCoreMtl() { return m_pCoreMtl; }

	IShader* GetRenderShader();
	void SetParamsToShader(IShader* pShader);

	void SetMtlParam(const Uint32 index, const Variant &var);
	void SetMtlParam(const String &name, const Variant &var);
	Bool GetMtlParam(const String &name, Variant &var);
	Bool GetMtlParam(const Uint32 index, Variant &var);
	Uint32 GetMtlParamCount(void) const;

	Bool GetMtlParamIndexByName(const String &name,Uint32& index);
	Bool GetMtlParamNameByIndex(const Uint32 index, String& name);

	ShaderMacro GetMtlMacro() const		{ return m_mtlmacro; }
	void SetMtlMacro(ShaderMacro macro)	{ m_mtlmacro = macro;	}

	inline String& GetMtlName()	{ return m_pShaderName;	}

	Material *Clone();

	void SetMtlSamplers(const String &name, FilterState filterstate, AddressState addrstate);
	Bool GetMtlSamplers(const String &name, FilterState& filterstate, AddressState& addrstate);
	Bool GetMtlSamplersIndexByName(const String &name, Uint32& index);

protected:
	void ReadParam(XmlNode *param_node);

private:
	IMaterialTemplate* m_pCoreMtl;
	ShaderMacro m_mtlmacro;
	RenderState m_mtlRenderState;

	std::vector<MtlParam> m_ParamArray;

	std::vector<MtlSam> m_matsamplers;

	String m_pShaderName;
	String m_techName;
	IShader *m_shader;

};
