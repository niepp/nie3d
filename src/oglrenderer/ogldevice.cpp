#include "core/core.h"
#include "ogl.h"
#include "ogldefine.h"
#include "ogldevice.h"

#if PLATFORM == PLATFORM_WIN32

typedef BOOL (WINAPI * PFNWGLwglChoosePixelFormatARBPROC) (HDC hdc,
    const int *piAttribIList,
    const FLOAT *pfAttribFList,
    UINT nMaxFormats,
    int *piFormats,
    UINT *nNumFormats);

typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);

static PFNWGLwglChoosePixelFormatARBPROC wglChoosePixelFormat = NULL;
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

PIXELFORMATDESCRIPTOR OGLDevice::m_pfd = { 
	sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
	1,                     // version number 
	PFD_DRAW_TO_WINDOW |   // support window 
	PFD_SUPPORT_OPENGL |   // support OpenGL 
	PFD_DOUBLEBUFFER,      // double buffered 
	PFD_TYPE_RGBA,         // RGBA type 
	32,                    // 32-bit color depth 
	0, 0, 0, 0, 0, 0,      // color bits ignored 
	0,                     // no alpha buffer 
	0,                     // shift bit ignored 
	0,                     // no accumulation buffer 
	0, 0, 0, 0,            // accum bits ignored 
	32,                    // 32-bit z-buffer 
	0,                     // no stencil buffer 
	0,                     // no auxiliary buffer 
	PFD_MAIN_PLANE,        // main layer 
	0,                     // reserved 
	0, 0, 0                // layer masks ignored 
};

Bool OGLDevice::Init(HDC hDC)
{
	HWND hWnd = CreateWindow ("STATIC", NULL,
		WS_OVERLAPPED,// ´°¿ÚÑùÊ½,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, NULL, NULL);
	HDC temphDC = GetDC(hWnd);

	Int32 nPixelFormat = ChoosePixelFormat(temphDC, &m_pfd);
	SetPixelFormat(temphDC, nPixelFormat, &m_pfd);

	HGLRC hTempRC = wglCreateContext(temphDC);
	wglMakeCurrent(temphDC, hTempRC);

	wglChoosePixelFormat = (PFNWGLwglChoosePixelFormatARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hTempRC);
	ReleaseDC(hWnd, temphDC);
	DestroyWindow(hWnd);

	int attributes[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,     24,
		WGL_ALPHA_BITS_ARB,     8,
		WGL_DEPTH_BITS_ARB,     24,
		WGL_STENCIL_BITS_ARB,   0,
		WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB,        4,
		0, 0
	};

	UINT numFormats = 0;

	if (!wglChoosePixelFormat(hDC, attributes, 0, 1, &m_nPixelFormat, &numFormats))
	{
		attributes[18] = 1;
		wglChoosePixelFormat(hDC, attributes, 0, 1, &m_nPixelFormat, &numFormats);
	}

	assert(m_nPixelFormat != -1);

	if (!SetPixelFormat(hDC, m_nPixelFormat, &m_pfd))
	{
		DWORD err = GetLastError();
		assert(false);
		return false;
	}

	GLint attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		0, 0
	};

	m_hRC = wglCreateContextAttribsARB(hDC, 0, attribs);

	if (m_hRC == NULL)
	{
		assert(false);
		return false;
	}

	wglMakeCurrent(hDC, m_hRC);

	GLenum err = glewInit();
	assert(err == GLEW_OK);

	const GLubyte* version = glGetString(GL_VERSION);
	printf("the Opengl version is:%s \n", version);

	return true;

}


#elif PLATFORM == PLATFORM_ANDROID

Bool OGLDevice::Init(ANativeWindow *native_window)
{
	/*
	* Here specify the attributes of the desired configuration.
	* Below, we select an EGLConfig with at least 8 bits per color
	* component compatible with on-screen windows
	*/
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_NONE
	};

	const EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLint dummy, format;
	EGLint numConfigs;

	m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(m_display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	* sample, we have a very simplified selection process, where we pick
	* the first EGLConfig that matches our criteria */
	eglChooseConfig(m_display, attribs, &m_config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	* guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	* As soon as we picked a EGLConfig, we can safely reconfigure the
	* ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(native_window, 0, 0, format);
	m_surface = eglCreateWindowSurface(m_display, m_config, native_window, NULL);
	m_context = eglCreateContext(m_display, m_config, NULL, contextAttribs);

	if (eglMakeCurrent(m_display, m_surface, m_surface, m_context) == EGL_FALSE)
	{
		return false;
	}

	return true;

}

void OGLDevice::Destroy()
{
	if (m_display != EGL_NO_DISPLAY) {
		eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (m_context != EGL_NO_CONTEXT) {
			eglDestroyContext(m_display, m_context);
		}
		if (m_surface != EGL_NO_SURFACE) {
			eglDestroySurface(m_display, m_surface);
		}
		eglTerminate(m_display);
	}

	m_surface = EGL_NO_SURFACE;
	m_context = EGL_NO_CONTEXT;
	m_display = EGL_NO_DISPLAY;

}

#endif


