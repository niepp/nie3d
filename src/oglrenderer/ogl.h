#pragma once

#if (PLATFORM == PLATFORM_WIN32)
#	if defined(OGLRENDERER_STATIC_LIB)
#		define		OGLRENDERER_API
#	else
#		if defined(OGLRENDERER_EXPORTS)
#			define	OGLRENDERER_API			__declspec(dllexport)
#		else
#			define	OGLRENDERER_API			__declspec(dllimport)
#		endif
#	endif
#else
#   if defined(OGLRENDERER_STATIC_LIB)
#       define		OGLRENDERER_API
#   else
#       if defined(OGLRENDERER_EXPORTS)
#           define  OGLRENDERER_API         __attribute__((visibility("default")))
#		else
#			define	OGLRENDERER_API
#		endif
#	endif
#endif

#if PLATFORM == PLATFORM_WIN32
#include "glew-1.9.0/include/GL/glew.h"
#include "gl/GL.h"
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#elif PLATFORM == PLATFORM_IOS
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif PLATFORM == PLATFORM_ANDROID
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

class GLES2Ext
{
public:
	static void InitExtension();

public:
	typedef void* (GL_APIENTRY *PFNGLMAPBUFFEROES)(GLenum target, GLenum access);
	typedef GLboolean (GL_APIENTRY *PFNGLUNMAPBUFFEROES)(GLenum target);

	typedef void (GL_APIENTRYP PFNGLGENVERTEXARRAYSOESPROC) (GLsizei n, GLuint *arrays);
	typedef void (GL_APIENTRYP PFNGLBINDVERTEXARRAYOESPROC) (GLuint array);
	typedef void (GL_APIENTRYP PFNGLDELETEVERTEXARRAYSOESPROC) (GLsizei n, const GLuint *arrays);

	static PFNGLMAPBUFFEROES            glMapBufferOES;
	static PFNGLUNMAPBUFFEROES          glUnmapBufferOES;

	static PFNGLGENVERTEXARRAYSOESPROC  glGenVertexArraysOES;
	static PFNGLBINDVERTEXARRAYOESPROC  glBindVertexArrayOES;
	static PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
};

#endif
