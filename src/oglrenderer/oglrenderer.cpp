#include "core/core.h"

#include "ogl.h"
#include "ogldefine.h"
#include "ogldevice.h"
#include "oglgeobuffer.h"
#include "oglinputlayout.h"
#include "ogltexture.h"
#include "oglmaterialtemplate.h"
#include "oglwindowtarget.h"
#include "oglrendertarget.h"
#include "oglrenderer.h"


#if PLATFORM == PLATFORM_ANDROID
GLES2Ext::PFNGLMAPBUFFEROES       GLES2Ext::glMapBufferOES = 0;
GLES2Ext::PFNGLUNMAPBUFFEROES     GLES2Ext::glUnmapBufferOES = 0;
GLES2Ext::PFNGLGENVERTEXARRAYSOESPROC       GLES2Ext::glGenVertexArraysOES = 0;
GLES2Ext::PFNGLBINDVERTEXARRAYOESPROC     GLES2Ext::glBindVertexArrayOES = 0;
GLES2Ext::PFNGLDELETEVERTEXARRAYSOESPROC       GLES2Ext::glDeleteVertexArraysOES = 0;

void GLES2Ext::InitExtension()
{
	const GLubyte *pszGLExtensions = glGetString(GL_EXTENSIONS);
	if (strstr((char *)pszGLExtensions, "GL_OES_mapbuffer"))
	{
		glMapBufferOES = (PFNGLMAPBUFFEROES) eglGetProcAddress("glMapBufferOES");
		glUnmapBufferOES = (PFNGLUNMAPBUFFEROES) eglGetProcAddress("glUnmapBufferOES");
	}
	else if (strstr((char *)pszGLExtensions, "OES_depth_texture"))
	{
		LogUtils::Instance()->LogInfo("support depth_texture extention!");
	}
	glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC) eglGetProcAddress("glGenVertexArraysOES");
	glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC) eglGetProcAddress("glBindVertexArrayOES");
	glDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSOESPROC) eglGetProcAddress("glDeleteVertexArraysOES");

}
#endif

OGLRenderer::OGLRenderer() : m_lastLayout(NULL)
{
	strcpy(m_shaderDir, "oglshaders/");
	m_curRenderState.SetDefault();
}

OGLRenderer::~OGLRenderer()
{

}

Bool OGLRenderer::Create(const RendererDesc &desc)
{
	if( !InitDevice(desc) )
	{
		return false;
	}

#if PLATFORM == PLATFORM_ANDROID
	GLES2Ext::InitExtension();
#endif
	return true;
}

void OGLRenderer::Destroy()
{
#if PLATFORM == PLATFORM_ANDROID
	m_device.Destroy();
	((OGLWinRenderTarget*)GetMainTarget())->DestroyDeviceObject();
#endif
}

Bool OGLRenderer::InitDevice(const RendererDesc &desc)
{
	OGLWinRenderTarget *ptarget = new OGLWinRenderTarget(desc);
	m_WindowTargets.push_back(ptarget);
#if PLATFORM == PLATFORM_WIN32
	if (!m_device.Init(ptarget->m_hDC))
	{
		return false;
	}
#elif PLATFORM == PLATFORM_ANDROID
	if (!m_device.Init(ptarget->m_native_window))
	{
		return false;
	}
#endif

	if(!ptarget->InitDeviceObject(&m_device))
	{
		m_WindowTargets.pop_back();
		delete ptarget;
		return false;
	}

	return true;

}

IMaterialTemplate *OGLRenderer::CreateMaterialTemplate(const char *path)
{
	assert(path);
	String name(path);
	MaterialTemplate *pmtl = FindMaterialTemplate(name);

	if(!pmtl && strstr(path, ".fx") != NULL)
	{
		pmtl = new MaterialTemplate(m_shaderDir, path);
		AddMaterialTemplate(name, pmtl);
	}

	return pmtl;
}

MaterialTemplate* OGLRenderer::FindMaterialTemplate(const String &name)
{
	MaterialTemplate *pmtl = NULL;
	MaterialTemplateMap::iterator itr = m_mtlTempMap.find(name);
	if (itr != m_mtlTempMap.end())
	{
		pmtl = itr->second;
	}
	return pmtl;
}

void OGLRenderer::AddMaterialTemplate(const String &name, MaterialTemplate *pcoremtl)
{
	m_mtlTempMap[name] = pcoremtl;
}

ITexture *OGLRenderer::CreateTexture(const TextureDesc &texdesc, const TextureData *ptexdata)
{
	OGLTexture *ptex = new OGLTexture;
	if(!ptex->Create(texdesc, ptexdata))
	{
		delete(ptex);
		return NULL;
	}
	return ptex;
}

IIndexBuffer *OGLRenderer::CreateIndexBuffer(UsageType usage, Uint32 num_index, IndexType index_type)
{
	Uint32 index_size = 4;
	switch(index_type)
	{
	case cIndex32:
		index_size = 4;
		break;
	case cIndex16:
		index_size = 2;
		break;
	case cIndex8:
		index_size = 1;
		break;
	default:
		assert(0);
		break;
	}

	return new OGLIndexBuffer(num_index * index_size, usage);

}

IVertexBuffer *OGLRenderer::CreateVertexBuffer(UsageType usage, Uint32 buflen, Uint32 stride)
{
	return new OGLVertexBuffer(buflen, usage, stride);
}

IInputLayout *OGLRenderer::CreateInputLayout(const InputElement *pelements, Uint32 nelement)
{
	return new OGLInputLayout(pelements, nelement);
}

void OGLRenderer::SetVertexBuffer(IVertexBuffer *pbuf, Uint32 offsetbytes, Uint32 stride)
{
	OGLVertexBuffer *ogl_vb = static_cast<OGLVertexBuffer*>(pbuf);
	GLBINDBUFFER(GL_ARRAY_BUFFER, ogl_vb->m_VertexBuffer);
}

void OGLRenderer::SetInputLayout(IInputLayout *playout)
{
	if (m_lastLayout != NULL) {
		static_cast<OGLInputLayout*>(m_lastLayout)->End();
	}

	OGLInputLayout *ogl_layout = static_cast<OGLInputLayout*>(playout);  
	ogl_layout->Begin();
	m_lastLayout = playout;
}

void OGLRenderer::Draw(PrimitiveType topo, Uint32 nvert, IIndexBuffer *pindexbuf, Uint32 startindex)
{
	GLenum gltopo = g_PrimTopology[topo];
	if(pindexbuf == NULL)
	{
		GLDRAWARRAYS(gltopo, startindex, nvert);
	}
	else
	{
		OGLIndexBuffer* pib = static_cast<OGLIndexBuffer*>(pindexbuf);
		GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER, pib->m_IndexBuffer);

		//void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)
		// 这个indices是个多面手，如果没有使用 VBO，它指向CPU内存中的索引数据数组；如果使用 VBO 绑定到 GL_ELEMENT_ARRAY_BUFFER，那么它表示索引数据在 VBO 中的偏移量。
		GLDRAWELEMENTS(gltopo, nvert, GL_UNSIGNED_SHORT, (void*)startindex);
	}
}

IRenderTarget* OGLRenderer::CreateRenderTarget(Uint32 width, Uint32 height)
{
	return new OGLRenderTarget(width, height);
}

IRenderTarget *OGLRenderer::GetMainTarget()
{
	assert(m_WindowTargets.size() > 0);
	IRenderTarget *ptarget = m_WindowTargets[0];
	return ptarget;
}

IRenderTarget *OGLRenderer::CreateAdditionalWindowTarget(const RendererDesc &desc)
{
	OGLWinRenderTarget *ptarget = new OGLWinRenderTarget(desc);
	if(ptarget->InitDeviceObject(&m_device))
	{
		m_WindowTargets.push_back(ptarget);
		return ptarget;
	}
	else
	{
		delete(ptarget);
		return NULL;
	}
}

Bool OGLRenderer::SetRenderState(RenderState rs)
{
	if(rs == m_curRenderState) {
		return true;
	}

	//blend renderstate
	if(rs.alpha_blend != m_curRenderState.alpha_blend) {
		if(rs.alpha_blend > 0) {
			GLENABLE(GL_BLEND);
			GLBLENDFUNC(g_BlendFactor[rs.src_blend], g_BlendFactor[rs.dest_blend]);
			GLBLENDEQUATION(g_BlendFunc[rs.blend_op]);
		}
		else {
			GLDISABLE(GL_BLEND);
		}
	}

	if(rs.src_blend != m_curRenderState.src_blend || rs.dest_blend != m_curRenderState.dest_blend)
	{
		GLBLENDFUNC(g_BlendFactor[rs.src_blend], g_BlendFactor[rs.dest_blend]);
	}

	if(rs.blend_op != m_curRenderState.blend_op)
	{
		GLBLENDEQUATION( g_BlendFunc[rs.blend_op]);
	}

	//depth renderstate
	if(rs.depth_test_enable != m_curRenderState.depth_test_enable)
	{
		if(rs.depth_test_enable > 0) {
			GLENABLE(GL_DEPTH_TEST);
		}
		else {
			GLDISABLE(GL_DEPTH_TEST);
		}
	}
 
	if(rs.depth_write_enable != m_curRenderState.depth_write_enable)
	{
		GLDEPTHMASK(rs.depth_write_enable);
	}

	if(rs.depth_func != m_curRenderState.depth_func)
	{
		GLDEPTHFUNC(g_CMPFunc[rs.depth_func]);
	}

	//stencil rendefstate
	if(rs.stencil_enable != m_curRenderState.stencil_enable)
	{
		if(rs.stencil_enable > 0) {
			GLENABLE(GL_STENCIL_TEST);
		}
		else {
			GLDISABLE(GL_STENCIL_TEST);
		}
	}

#if PLATFORM != PLATFORM_IOS && PLATFORM != PLATFORM_ANDROID
	//FillMode renderstate
	if(rs.fill_mode != m_curRenderState.fill_mode)
	{
		GLPOLYGONMODE(GL_FRONT_AND_BACK, g_Fillmode[rs.fill_mode]);	
	}
#endif

	//cullmode renderstate
	if(rs.cull_mode != m_curRenderState.cull_mode)
	{
		if(rs.cull_mode == cCullNone) {
			GLDISABLE(GL_CULL_FACE);
		}
		else if(rs.cull_mode == cCullCW) {
			GLENABLE(GL_CULL_FACE);
			GLCULLFACE(GL_FRONT);
		}
		else if(rs.cull_mode == cCullCCW) {
			GLENABLE(GL_CULL_FACE);
			GLCULLFACE(GL_BACK);
		}
		else {
			assert(0);
		}
	}

	m_curRenderState = rs;

	return true;

}

void OGLRenderer::RestoreDefaultState(Bool samplerState)
{
	TestGLError();
	GLCOLORMASK(true, true, true, true);

	GLDISABLE(GL_BLEND);
	GLBLENDFUNC(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GLBLENDEQUATION(GL_FUNC_ADD);

	GLENABLE(GL_DEPTH_TEST);
	GLDEPTHMASK(true);
	GLDEPTHFUNC(GL_LEQUAL);
	GLDISABLE(GL_STENCIL_TEST);

	GLENABLE(GL_CULL_FACE);
	GLCULLFACE(GL_BACK);

#if PLATFORM != PLATFORM_IOS && PLATFORM != PLATFORM_ANDROID
	GLDISABLE(GL_ALPHA_TEST);
	GLALPHAFUNC(GL_GREATER, 0.5);
	GLPOLYGONMODE(GL_FRONT_AND_BACK, GL_FILL);
	GLENABLE(GL_MULTISAMPLE);
	GLENABLE(GL_LINE_SMOOTH);
	GLHINT(GL_LINE_SMOOTH_HINT, GL_NICEST);
#endif

	TestGLError();

	m_curRenderState.SetDefault();

}

OGLWinRenderTarget* OGLRenderer::FindWindowTarget(HWND hwnd)
{
#if PLATFORM == PLATFORM_WIN32
	for(Uint32 i = 0; i < m_WindowTargets.size(); ++i)
	{
		if(m_WindowTargets[i]->m_hWnd == hwnd)
		{
			return m_WindowTargets[i];
		}
	}
#endif
	return NULL;
}

OGLRENDERER_API IRenderer* CreateRenderer(const RendererDesc &desc)
{
	OGLRenderer *prenderer = new OGLRenderer;
	if(prenderer->Create(desc)) {
		return prenderer;
	}
	else {
		prenderer->Destroy();
		delete(prenderer);
		return NULL;
	}
}
