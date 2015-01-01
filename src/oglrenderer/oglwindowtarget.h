#pragma once
#include <vector>

class OGLWinRenderTarget : public IRenderTarget
{
public:
	OGLWinRenderTarget(const RendererDesc &desc);

	virtual Bool BeginScene(Uint32 index = 0);
	virtual void EndScene(Uint32 index = 0);
	virtual void Present();
	virtual void GetSize(Uint32 &width, Uint32 &height)
	{
		width = (Uint32)m_rendererDesc.width;
		height = (Uint32)m_rendererDesc.height;
	}

	void SetClearParam(Uint32 flags, const Color &color, Float z, Uint32 stencil)
	{
		m_ClearFlags = flags;
		m_ClearColor = color;
		m_ClearDepth = z;
		m_ClearStencil = stencil;
	}

	virtual void HandlePossibleSizeChange();

	Bool InitDeviceObject(OGLDevice *device);
	void DestroyDeviceObject();

	virtual void AttachTexture(ITexture* pDepthTexture, Uint32 DSLevel,	ITexture* pRTTexture, Uint32 RTTLevel)
	{
		assert(0);
	}

	virtual Bool Lock(LockResult& outLesult, const RectInt* inRect = NULL) { return false; }
	virtual void Unlock(){}

public:
	virtual ~OGLWinRenderTarget();
#if PLATFORM == PLATFORM_WIN32
	HWND m_hWnd;
	HDC	m_hDC;
#elif PLATFORM == PLATFORM_ANDROID
	ANativeWindow *m_native_window;
	EGLSurface m_surface;
	EGLContext m_context;
	EGLDisplay m_display;
#endif

	RendererDesc m_rendererDesc;
	Uint32 m_ClearFlags;
	Color m_ClearColor;
	Float m_ClearDepth;
	Uint32 m_ClearStencil;

};
