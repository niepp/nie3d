#include "core/core.h"
#include "ogl.h"
#include "ogldefine.h"
#include "ogltexture.h"
#include "oglrendertarget.h"

OGLRenderTarget::OGLRenderTarget(Uint32 width, Uint32 height)
    : m_pInternalLockBuffer(NULL)
	, m_Width(width)
	, m_Height(height)
	, m_FrameBuffer(0)
	, m_DepthBuffer(0)
	, m_ClearFlags(0)
	, m_ClearColor(Color::Black)
	, m_ClearDepth(1.0f)
	, m_ClearStencil(0)
{
	GLGENBUFFERS(1, &m_FrameBuffer);
}

OGLRenderTarget::~OGLRenderTarget()
{
    GLDELETEFRMAEBUFFERS(1, &m_FrameBuffer);
	if (m_DepthBuffer) {
		GLDELETEFRMAEBUFFERS(1, &m_DepthBuffer);
	}
}

Bool OGLRenderTarget::BeginScene(Uint32 index)
{
	TestGLError();
	GLBINDFRAMEBUFFER(GLFRAMEBUFFER, m_FrameBuffer);
	if (m_DepthBuffer) {
		GLBINDRENDERBUFFER(GLRENDERBUFFER, m_DepthBuffer);
	}

	//clear
	if(m_ClearFlags > 0) {
		GLbitfield mask = 0;
		if(m_ClearFlags & cClearStencil) mask |= GL_STENCIL_BUFFER_BIT;
		if(m_ClearFlags & cClearTarget) mask |= GL_COLOR_BUFFER_BIT;
		if(m_ClearFlags & cClearZBuffer) mask |= GL_DEPTH_BUFFER_BIT;

		if(m_ClearFlags & cClearTarget) {
			Color clrColor = m_ClearColor;
			GLCLEARCOLOR(clrColor.r, clrColor.g, clrColor.b, clrColor.a);
		}

		if(m_ClearFlags & cClearZBuffer) {
            GLCLEARDEPTH(m_ClearDepth);
		}
		if(m_ClearFlags & cClearStencil) {
			GLCLEARSTENCIL(m_ClearStencil);
		}
		GLCLEAR(mask);
	}
	GLGETINTEGERV(GL_VIEWPORT , m_originViewPort);
	GLVIEWPORT(0, 0, m_Width, m_Height);
	
    return true;
}

void OGLRenderTarget::EndScene(Uint32 index)
{
	GLVIEWPORT(m_originViewPort[0], m_originViewPort[1], m_originViewPort[2], m_originViewPort[3]);
	GLBINDFRAMEBUFFER(GLFRAMEBUFFER, 0);
	GLBINDRENDERBUFFER(GLRENDERBUFFER, 0);
}

void OGLRenderTarget::Present()
{
	assert(0);
}

void OGLRenderTarget::AttachTexture(ITexture *depth_tex, Uint32 depth_level, ITexture *rt_tex, Uint32 rt_level)
{
	assert(rt_tex != NULL || depth_tex != NULL);

	GLBINDFRAMEBUFFER(GLFRAMEBUFFER, m_FrameBuffer);

	if(depth_tex != NULL) {
		Uint32 w = 0, h = 0;
		depth_tex->GetSize(w, h);
		assert(w == m_Width && h == m_Height);
		OGLTexture *depth = static_cast<OGLTexture*>(depth_tex);
		GLFRAMEBUFFERTEXTURE(GLFRAMEBUFFER, GLDEPTHATTACHMENT, GL_TEXTURE_2D, depth->GetGLTex(), 0);
		TestGLError();
	}
	else{
		// Create depth renderbuffer
		if(m_ClearFlags & cClearZBuffer) {
			GLGENBUFFERS(1, &m_DepthBuffer);
			GLBINDRENDERBUFFER(GLRENDERBUFFER, m_DepthBuffer);
			GLRENDERBUFFERSTORAGE(GLRENDERBUFFER, GLDEPTHCOMPONENT, m_Width, m_Height);
			GLFRAMEBUFFERRENDERBUFFER(GLFRAMEBUFFER, GLDEPTHATTACHMENT, GL_RENDERBUFFER, m_DepthBuffer);
            TestGLError();
		}
	}

	if(rt_tex != NULL) {
		Uint32 w = 0, h = 0;
		rt_tex->GetSize(w, h);
		assert(w == m_Width && h == m_Height);
		OGLTexture *target = static_cast<OGLTexture*>(rt_tex);
		GLFRAMEBUFFERTEXTURE(GLFRAMEBUFFER, GLCOLORATTACHMENT0, GL_TEXTURE_2D, target->GetGLTex(), 0);
		TestGLError();
	}
	else {
#if PLATFORM != PLATFORM_IOS && PLATFORM != PLATFORM_ANDROID
		glDrawBuffer(GL_NONE);
#endif
	}

	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(err == GL_FRAMEBUFFER_COMPLETE);

}

Bool OGLRenderTarget::Lock(LockResult &outLesult, const RectInt *inRect)
{
    GLBINDFRAMEBUFFER(GLFRAMEBUFFER, m_FrameBuffer);
#if PLATFORM != PLATFORM_IOS && PLATFORM != PLATFORM_ANDROID
    glReadBuffer(GLCOLORATTACHMENT0);
#endif
    int w = m_Width;
    int h = m_Height;
    int x = 0;
    int y = 0;

    if (inRect != NULL)
    {
        w = inRect->right - inRect->left;
        h = inRect->bottom - inRect->top;
        x = inRect->left;
        y = inRect->top;
    }

    outLesult.pitch = 4 * w;
    if (outLesult.bits == NULL)
    {
        m_pInternalLockBuffer = new Int8[outLesult.pitch * h];
        outLesult.bits = m_pInternalLockBuffer;
    }

#if PLATFORM == PLATFORM_ANDROID
	glReadPixels(x, y, w, h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, outLesult.bits);
#else
	glReadPixels(x, y, w, h, GL_BGRA, GL_UNSIGNED_BYTE, outLesult.bits);
#endif
    return true;
}

void OGLRenderTarget::Unlock()
{
    if (m_pInternalLockBuffer != NULL)
    {
        delete[] m_pInternalLockBuffer;
        m_pInternalLockBuffer = NULL;
    }
}

void OGLRenderTarget::GetSize(Uint32& fWidth, Uint32& fHeight)
{
	fWidth = m_Width;
	fHeight = m_Height;
}

void OGLRenderTarget::SetClearParam(Uint32 flags, const Color &color, Float z, Uint32 stencil)
{
	m_ClearFlags = flags;
	m_ClearColor = color;
	m_ClearDepth = z;
	m_ClearStencil = stencil;
}

void OGLRenderTarget::HandlePossibleSizeChange()
{

}
