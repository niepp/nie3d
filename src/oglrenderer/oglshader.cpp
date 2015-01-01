#include "core/core.h"
#include "ogl.h"
#include "ogldefine.h"
#include "ogltexture.h"
#include "oglgeobuffer.h"
#include "oglinputlayout.h"
#include "oglshader.h"

OGLShader::OGLShader() 
    : m_VertexShader(0)
    , m_PixelShader(0)
    , m_ShaderProgram(0)
{
}

OGLShader::~OGLShader()
{
	//delete shader and program here
}

void OGLShader::DeleteShader()
{
    GLDELETEPROGRAM(m_ShaderProgram);
    GLDELETESHADER(m_VertexShader);
    GLDELETESHADER(m_PixelShader);
}

void OGLShader::BindAttrib()
{
	GLBINDATTRIBLOCATION(m_ShaderProgram, cInputUsagePosition,		"vPos");
	GLBINDATTRIBLOCATION(m_ShaderProgram, cInputUsageBlendWeight,	"blendweights");
	GLBINDATTRIBLOCATION(m_ShaderProgram, cInputUsageBlendIndices,	"blendindices");
	GLBINDATTRIBLOCATION(m_ShaderProgram, cInputUsageNormal,		"vNormal");
	GLBINDATTRIBLOCATION(m_ShaderProgram, cInputUsageTangent,		"vTangent");
	GLBINDATTRIBLOCATION(m_ShaderProgram, cInputUsageBinormal,		"vBinormal");
	GLBINDATTRIBLOCATION(m_ShaderProgram, cInputUsageColor,			"vColor");
	GLBINDATTRIBLOCATION(m_ShaderProgram, cInputUsageTexcoord,		"vTexCoord0");
	GLBINDATTRIBLOCATION(m_ShaderProgram, cInputUsageCount + 0,		"vTexCoord1");
}

GLuint OGLShader::CompileShader(const char* shaderSource,const int& sourceLength, const GLenum& shadertype,
							const char* shaderEntry,const char* pshaderMacro[],	const int& macrosize)
{
	GLuint shader;
	char* vertexshaderbuf = new char[sourceLength + 1];
	strncpy(vertexshaderbuf, shaderSource, sourceLength);
	vertexshaderbuf[sourceLength] = '\0';
	String vertexstr = vertexshaderbuf;
	Uint32 index = vertexstr.FindStr(shaderEntry);
    assert(index != cInvalidIndex);
	if( index != cInvalidIndex)
	{
		shader = GLCREATESHADER(shadertype);
		const char* buf0 = vertexshaderbuf + index + strlen(shaderEntry);
		vertexshaderbuf[index] = 0;
		String finalvertexstring = String(vertexshaderbuf) + String("main") + String(buf0);

#if PLATFORM == PLATFORM_WIN32
		finalvertexstring += String(" \n");
#else
		finalvertexstring += String(" \r\n");
#endif

		const char** pshaderstr = new const char*[macrosize + 3];
		int i = 0;
#if PLATFORM == PLATFORM_WIN32
        pshaderstr[0] = "#define WINDOWS 1 \n";
#else
		pshaderstr[0] = "#define MOBILE 1 \r\n";
#endif
		for(i = 0; i < macrosize; ++i) {
			pshaderstr[i + 1] = (const char*)pshaderMacro[i];
		}

        if(shadertype == GL_VERTEX_SHADER) {
#if PLATFORM == PLATFORM_WIN32
            pshaderstr[macrosize + 1] = "#define VS 1 \n";
#else
			pshaderstr[macrosize + 1] = "#define VS 1 \r\n";
#endif
		}
		else if(shadertype == GL_FRAGMENT_SHADER) {	
#if PLATFORM == PLATFORM_WIN32
			pshaderstr[macrosize + 1] = "#define PS 1 \n";
#else
			pshaderstr[macrosize + 1] = "#define PS 1 \r\n";
#endif
		}
		else {
			assert(0);
		}

		pshaderstr[macrosize + 2] = (const char*)finalvertexstring.AsCharPtr();
		
		GLSHADERSOURCE(shader, macrosize + 3, (const GLchar **)pshaderstr, NULL);
		GLCOMPILESHADER(shader);
		GLint compiled;
		GLGETSHADERIV(shader, GL_COMPILE_STATUS, &compiled);

		delete[] pshaderstr;
		if(compiled == GL_FALSE)
		{
			GLint length;
			GLGETSHADERIV(shader,GL_INFO_LOG_LENGTH,&length);
			GLchar* log = new GLchar[length];
			GLGETSHADERINFOLOG(shader, length, &length, log);
            LogUtils::Instance()->LogInfo(log);
			delete[] log;
			delete[] vertexshaderbuf;
			return 0;
		}
		else
		{
			delete[] vertexshaderbuf;
			return shader;
		}

	}
	
	delete[] vertexshaderbuf;
	return 0;

}

Bool OGLShader::BuildShader(const char *pShaderFile, const Uint32& buflen, 
						  const char* pShadermacros[], const Uint32& size,
						  const char* pvsEntry, const char* pfsEntry)
{
	TestGLError();

	m_VertexShader = CompileShader(pShaderFile, buflen,
		GL_VERTEX_SHADER, pvsEntry, pShadermacros, size);

	m_PixelShader = CompileShader(pShaderFile, buflen,
		GL_FRAGMENT_SHADER,	pfsEntry, pShadermacros, size);

	m_ShaderProgram = GLCREATEPROGRAM();
	GLATTACHSHADER(m_ShaderProgram, m_VertexShader);
	GLATTACHSHADER(m_ShaderProgram, m_PixelShader);
	BindAttrib();
	GLLINKPROGRAM(m_ShaderProgram);

	GLint linked = 0;
	GLGETPROGRAMIV(m_ShaderProgram, GL_LINK_STATUS, &linked);

	if(!linked)
	{
		GLint length(0);
		GLGETPROGRAMIV(m_ShaderProgram, GL_INFO_LOG_LENGTH, &length);
		GLchar* log = new GLchar[length];
		GLGETPROGRAMINFOLOG(m_ShaderProgram, length, &length, log);
		LogUtils::Instance()->LogInfo(log);
		delete[] log;
		return false;
	}

	Int32 constantCount;
	GLGETPROGRAMIV(m_ShaderProgram, GL_ACTIVE_UNIFORMS, &constantCount);

	Uint32 texindex = 0;
	for(Int32 i = 0; i < constantCount; ++i)
	{
		GLint length;
		GLint size;
		GLenum type;
		GLchar name[256];
		GLGETACTIVEUNIFORM(m_ShaderProgram, i, 256, &length, &size, &type, name);

		ConstantInfo constantInfo;
		constantInfo.name = name;
		constantInfo.size = size;
		constantInfo.type = type;
		constantInfo.textureindex = 0;

		constantInfo.location = GLGETUNIFORMLOCATION(m_ShaderProgram, name);
		if(type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE)
		{
			GLuint location = GLGETUNIFORMLOCATION(m_ShaderProgram, name);
			constantInfo.textureindex = texindex;
            constantInfo.location = location;
			texindex++;
		}

		m_ConstantTable.push_back(constantInfo);
        int pos = constantInfo.name.FindChar('[');
        String strName;
        if (pos!= cInvalidIndex) {
            strName = constantInfo.name.SubString(0,pos);
        }
        else {
            strName = constantInfo.name;
        }
        m_mapConstantMap[strName] = i;
	}

	TestGLError();

	return true;
}

Uint32 OGLShader::Begin()
{
	return 0;
}

void OGLShader::End()
{
	
}

void OGLShader::BeginPass(Uint32 ipass)
{
	GLUSEPROGRAM(m_ShaderProgram);
}

void OGLShader::EndPass()
{
	GLUSEPROGRAM(0);
}

const OGLShader::ConstantInfo* OGLShader::GetConstantInfo(const String &name)
{
    std::map<String, Uint32>::iterator iter = m_mapConstantMap.find(name);
    if(iter != m_mapConstantMap.end())
    {
        Uint32 index = iter->second;
        if (index < m_ConstantTable.size()) {
            return &m_ConstantTable[index];
        }
    }
	return NULL;
}

Bool OGLShader::SetConstant(const ConstantInfo *info, const void *pdata)
{
	if(info == NULL) {
		return false;
	}

	switch(info->type)
	{
	case GL_FLOAT:
		GLUNIFORM1FV(info->location,info->size,(const GLfloat*)pdata);
		break;
	case GL_FLOAT_VEC2:
		GLUNIFORM2FV(info->location,info->size,(const GLfloat*)pdata);
		break;
	case GL_FLOAT_VEC3:
		GLUNIFORM3FV(info->location,info->size,(const GLfloat*)pdata);
		break;
	case GL_FLOAT_VEC4:
		GLUNIFORM4FV(info->location,info->size,(const GLfloat*)pdata);
		break;
	case GL_FLOAT_MAT4:
		GLUNIFORMMATRIX4FV(info->location,info->size,false,(const GLfloat*)pdata);
		break;
	case GL_SAMPLER_2D:
	case GL_SAMPLER_CUBE:
		GLACTIVETEXTURE(GL_TEXTURE0+info->textureindex);
		OGLTexture* pOGLTex;
		pOGLTex = (OGLTexture*)(*(Uint32*)pdata);
		GLBINDTEXTURE(GL_TEXTURE_2D, pOGLTex->GetGLTex());
		GLUNIFORM1I(info->location, info->textureindex);
		break;
	default:
		assert(0);
	}
	return true;
}

Bool OGLShader::SetConstant(const String &name, const void *pdata)
{
    const ConstantInfo* info = GetConstantInfo(name);
	return SetConstant(info, pdata);	
}

Bool OGLShader::SetTexture(const String &name, ITexture *ptexture)
{
    const ConstantInfo* info = GetConstantInfo(name);
    if (NULL==info) {
        return false;
    }

    GLACTIVETEXTURE(GL_TEXTURE0 + info->textureindex);
    OGLTexture* pOGLTex = static_cast<OGLTexture*>(ptexture);
#if PLATFORM != PLATFORM_IOS && PLATFORM != PLATFORM_ANDROID
	TextureDesc desc;
	ptexture->GetDesc(desc);
	if (desc.type == cTexture1D)
	{
		GLBINDTEXTURE(GL_TEXTURE_1D,pOGLTex->GetGLTex());
	}
	else if (desc.type == cTexture2D)
	{
		GLBINDTEXTURE(GL_TEXTURE_2D,pOGLTex->GetGLTex());
	}
#else
	GLBINDTEXTURE(GL_TEXTURE_2D, pOGLTex->GetGLTex());
#endif
    GLUNIFORM1I(info->location, info->textureindex);
	return true;
}
