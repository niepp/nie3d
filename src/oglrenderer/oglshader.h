#pragma once

#include <vector>

class OGLShader : public IShader
{
public:
	struct ConstantInfo
	{
		String name;
		GLenum type;
		GLint size;
		GLuint location;
		Uint32 textureindex;
	};

public:
	OGLShader();
	~OGLShader();

	Bool BuildShader(const char *pShaderFile, const Uint32& buflen, 
				    const char *pShadermacros[], const Uint32& size,
					const char* pvsEntry, const char* pfsEntry);

	virtual Uint32 Begin();
	virtual void End();
	virtual void BeginPass(Uint32 ipass = 0);
	virtual void EndPass();

    virtual Bool SetConstant(const String &name, const void *pdata);		
	virtual Bool SetTexture(const String &name, ITexture *ptexture);

private:
	GLuint CompileShader(const char* shaderSource,const int& sourceLength, const GLenum& shadertype,
					const char* shaderEntry, const char* pshaderMacro[], const int& macrosize);

	const ConstantInfo* GetConstantInfo(const String &name);
	Bool SetConstant(const ConstantInfo *info, const void *pdata);
	void BindAttrib();
	void DeleteShader();

private:
	GLuint m_VertexShader;
	GLuint m_PixelShader;
	GLuint m_ShaderProgram;

	std::vector<ConstantInfo> m_ConstantTable;
    std::map<String, Uint32> m_mapConstantMap;

};

