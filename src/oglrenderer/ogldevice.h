#pragma once

#if PLATFORM == PLATFORM_WIN32
class OGLDevice
{
public:
	OGLDevice() : m_hRC(NULL), m_nPixelFormat(0) { }
	Bool Init(HDC hDC);
public:
	Int32 m_nPixelFormat;
	HGLRC m_hRC;
	static PIXELFORMATDESCRIPTOR m_pfd;
};

#elif PLATFORM == PLATFORM_ANDROID
class OGLDevice
{
public:
    OGLDevice() : m_surface(0), m_context(0), m_display(0) { }
	Bool Init(ANativeWindow *native_window);
	void Destroy();
public:
	EGLSurface m_surface;
	EGLContext m_context;
	EGLDisplay m_display;
	EGLConfig m_config;
};

#elif PLATFORM == PLATFORM_IOS
class OGLDevice
{
public:
	OGLDevice() { }
};

#endif
