#include "core/core.h"
#include "ogl.h"
#include "ogldefine.h"
#include "ogldevice.h"
#include "oglgeobuffer.h"
#include "oglinputlayout.h"
#include "ogltexture.h"
#include "oglwindowtarget.h"

#if PLATFORM == PLATFORM_IOS
#include "core/ios/GLKViewObjectInterface.h"
#endif

OGLWinRenderTarget::OGLWinRenderTarget(const RendererDesc &desc)
	: m_rendererDesc(desc)
	, m_ClearFlags(cClearZBuffer| cClearTarget)
	, m_ClearColor(Color::Black)
	, m_ClearDepth(1.0f)
	, m_ClearStencil(0)
{
#if PLATFORM == PLATFORM_WIN32
	m_hWnd = (HWND)desc.context_obj;
	m_hDC = GetDC(m_hWnd);
#elif PLATFORM == PLATFORM_ANDROID
	m_native_window = (ANativeWindow*)desc.context_obj;
	m_surface = EGL_NO_SURFACE;
	m_context = EGL_NO_CONTEXT;
	m_display = EGL_NO_DISPLAY;
#endif
}

OGLWinRenderTarget::~OGLWinRenderTarget()
{
#if PLATFORM == PLATFORM_WIN32
	DestroyDeviceObject();
	if(m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
	}
#endif
}

Bool OGLWinRenderTarget::InitDeviceObject(OGLDevice *device)
{
#if PLATFORM == PLATFORM_WIN32
	if (!SetPixelFormat(m_hDC, device->m_nPixelFormat, &device->m_pfd))
    {
        assert(false);
        return false;
    }
    wglMakeCurrent(m_hDC, device->m_hRC);
#elif PLATFORM == PLATFORM_ANDROID
	m_surface = device->m_surface;
	m_context = device->m_context;
	m_display = device->m_display;
#endif
	return true;
}

void OGLWinRenderTarget::DestroyDeviceObject()
{
#if PLATFORM == PLATFORM_ANDROID
	m_surface = EGL_NO_SURFACE;
	m_context = EGL_NO_CONTEXT;
	m_display = EGL_NO_DISPLAY;
#endif
}

void OGLWinRenderTarget::HandlePossibleSizeChange()
{
#if PLATFORM == PLATFORM_WIN32
	RECT rcWindowClient;
	GetClientRect(m_hWnd, &rcWindowClient);
	m_rendererDesc.width = rcWindowClient.right - rcWindowClient.left;
	m_rendererDesc.height = rcWindowClient.bottom - rcWindowClient.top;
#endif
}

Bool OGLWinRenderTarget::BeginScene(Uint32 index)
{
#if PLATFORM == PLATFORM_IOS
    bindFrameBuffer(m_rendererDesc.context_obj);
#endif

    TestGLError();
    GLVIEWPORT(0, 0, m_rendererDesc.width, m_rendererDesc.height);

	if(m_ClearFlags != 0) {
		GLbitfield mask = 0;
		if(m_ClearFlags & cClearStencil) mask |= GL_STENCIL_BUFFER_BIT;
		if(m_ClearFlags & cClearTarget)	 mask |= GL_COLOR_BUFFER_BIT;
		if(m_ClearFlags & cClearZBuffer) mask |= GL_DEPTH_BUFFER_BIT;

		if(m_ClearFlags & cClearTarget)	{
			GLCLEARCOLOR(m_ClearColor.r / 255.0f
				, m_ClearColor.g / 255.0f
				, m_ClearColor.b / 255.0f
				, m_ClearColor.a / 255.0f);
		}

		if(m_ClearFlags & cClearZBuffer) {
			GLCLEARDEPTH(m_ClearDepth);
		}

		if(m_ClearFlags & cClearStencil) {
			GLCLEARSTENCIL(m_ClearStencil);
		}

		GLCLEAR(mask);

	}
    
    TestGLError();

	return true;
}

void OGLWinRenderTarget::EndScene(Uint32 index)
{
}

void OGLWinRenderTarget::Present()
{
#if PLATFORM == PLATFORM_WIN32
	SwapBuffers(m_hDC);
#elif PLATFORM == PLATFORM_ANDROID
	if (m_display != EGL_NO_DISPLAY) {
		eglSwapBuffers(m_display, m_surface);
	}
#endif

}
