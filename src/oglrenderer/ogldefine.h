#pragma once

#if PLATFORM == PLATFORM_WIN32

#define WGL_SAMPLE_BUFFERS_ARB             0x2041
#define WGL_SAMPLES_ARB                    0x2042

	// WGL_ARB_pixel_format

#define WGL_NUMBER_PIXEL_FORMATS_ARB       0x2000
#define WGL_DRAW_TO_WINDOW_ARB             0x2001
#define WGL_DRAW_TO_BITMAP_ARB             0x2002
#define WGL_ACCELERATION_ARB               0x2003
#define WGL_NEED_PALETTE_ARB               0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB        0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB         0x2006
#define WGL_SWAP_METHOD_ARB                0x2007
#define WGL_NUMBER_OVERLAYS_ARB            0x2008
#define WGL_NUMBER_UNDERLAYS_ARB           0x2009
#define WGL_TRANSPARENT_ARB                0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB      0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB    0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB     0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB    0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB    0x203B
#define WGL_SHARE_DEPTH_ARB                0x200C
#define WGL_SHARE_STENCIL_ARB              0x200D
#define WGL_SHARE_ACCUM_ARB                0x200E
#define WGL_SUPPORT_GDI_ARB                0x200F
#define WGL_SUPPORT_OPENGL_ARB             0x2010
#define WGL_DOUBLE_BUFFER_ARB              0x2011
#define WGL_STEREO_ARB                     0x2012
#define WGL_PIXEL_TYPE_ARB                 0x2013
#define WGL_COLOR_BITS_ARB                 0x2014
#define WGL_RED_BITS_ARB                   0x2015
#define WGL_RED_SHIFT_ARB                  0x2016
#define WGL_GREEN_BITS_ARB                 0x2017
#define WGL_GREEN_SHIFT_ARB                0x2018
#define WGL_BLUE_BITS_ARB                  0x2019
#define WGL_BLUE_SHIFT_ARB                 0x201A
#define WGL_ALPHA_BITS_ARB                 0x201B
#define WGL_ALPHA_SHIFT_ARB                0x201C
#define WGL_ACCUM_BITS_ARB                 0x201D
#define WGL_ACCUM_RED_BITS_ARB             0x201E
#define WGL_ACCUM_GREEN_BITS_ARB           0x201F
#define WGL_ACCUM_BLUE_BITS_ARB            0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB           0x2021
#define WGL_DEPTH_BITS_ARB                 0x2022
#define WGL_STENCIL_BITS_ARB               0x2023
#define WGL_AUX_BUFFERS_ARB                0x2024
#define WGL_NO_ACCELERATION_ARB            0x2025
#define WGL_GENERIC_ACCELERATION_ARB       0x2026
#define WGL_FULL_ACCELERATION_ARB          0x2027
#define WGL_SWAP_EXCHANGE_ARB              0x2028
#define WGL_SWAP_COPY_ARB                  0x2029
#define WGL_SWAP_UNDEFINED_ARB             0x202A
#define WGL_TYPE_RGBA_ARB                  0x202B
#define WGL_TYPE_COLORINDEX_ARB            0x202C

#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094

#endif


#if PLATFORM == PLATFORM_MAC
	#define  GLGENVERTEXARRAYS		glGenVertexArraysAPPLE
	#define  GLBINDVERTEXARRAY		glBindVertexArrayAPPLE
	#define  GLDELETEVERTEXARRAYS	glDeleteVertexArraysAPPLE
#elif PLATFORM == PLATFORM_IOS
	#define  GLGENVERTEXARRAYS		glGenVertexArraysOES
	#define  GLBINDVERTEXARRAY		glBindVertexArrayOES
	#define  GLDELETEVERTEXARRAYS	glDeleteVertexArraysOES
#elif PLATFORM == PLATFORM_ANDROID
    #define  GLGENVERTEXARRAYS		GLES2Ext::glGenVertexArraysOES
    #define  GLBINDVERTEXARRAY		GLES2Ext::glBindVertexArrayOES
    #define  GLDELETEVERTEXARRAYS	GLES2Ext::glDeleteVertexArraysOES
#else
	#define  GLGENVERTEXARRAYS		glGenVertexArrays
	#define  GLBINDVERTEXARRAY		glBindVertexArray
	#define  GLDELETEVERTEXARRAYS	glDeleteVertexArrays
#endif


#if PLATFORM == PLATFORM_IOS
	#define GLMAPBUFFER     glMapBufferOES
	#define GLUNMAPBUFFER   glUnmapBufferOES
	#define GLWRITEONLY     GL_WRITE_ONLY_OES
	#define GLREADONLY      GL_WRITE_ONLY_OES // see http://www.khronos.org/registry/gles/extensions/OES/OES_mapbuffer.txt
	#define GLREADWRITE     GL_WRITE_ONLY_OES
#elif PLATFORM == PLATFORM_ANDROID
	#define GLMAPBUFFER     GLES2Ext::glMapBufferOES
	#define GLUNMAPBUFFER   GLES2Ext::glUnmapBufferOES
	#define GLWRITEONLY     GL_WRITE_ONLY_OES
	#define GLREADONLY      GL_WRITE_ONLY_OES
	#define GLREADWRITE     GL_WRITE_ONLY_OES
#else
	#define GLMAPBUFFER     glMapBuffer
	#define GLUNMAPBUFFER   glUnmapBuffer
	#define GLWRITEONLY     GL_WRITE_ONLY
	#define GLREADONLY      GL_READ_ONLY
	#define GLREADWRITE     GL_READ_WRITE
#endif

#if PLATFORM == PLATFORM_IOS || PLATFORM == PLATFORM_ANDROID
	#define GLDELETEFRMAEBUFFERS    glDeleteFramebuffers
	#define GLBINDFRAMEBUFFER       glBindFramebuffer
	#define GLFRAMEBUFFER           GL_FRAMEBUFFER
	#define GLCOLORATTACHMENT0      GL_COLOR_ATTACHMENT0
	#define GLDEPTHATTACHMENT       GL_DEPTH_ATTACHMENT
	#define GLFRAMEBUFFERTEXTURE    glFramebufferTexture2D
    #define GLGENRENDERBUFFERS      glGenRenderbuffers
    #define GLBINDRENDERBUFFER      glBindRenderbuffer
    #define GLRENDERBUFFER          GL_RENDERBUFFER
	#define GLDEPTHCOMPONENT		GL_DEPTH_COMPONENT16
    #define GLRENDERBUFFERSTORAGEMULTISAMPLE  glRenderbufferStorageMultisample
    #define GLRENDERBUFFERSTORAGE             glRenderbufferStorage
    #define GLFRAMEBUFFERRENDERBUFFER         glFramebufferRenderbuffer
    #define GLREADFRAMEBUFFER                 GL_READ_FRAMEBUFFER
    #define GLDRAWFRAMEBUFFER                 GL_DRAW_FRAMEBUFFER
#else
	#define GLDELETEFRMAEBUFFERS    glDeleteFramebuffers
	#define GLBINDFRAMEBUFFER       glBindFramebufferEXT
	#define GLFRAMEBUFFER           GL_FRAMEBUFFER_EXT
	#define GLCOLORATTACHMENT0      GL_COLOR_ATTACHMENT0_EXT
	#define GLDEPTHATTACHMENT       GL_DEPTH_ATTACHMENT_EXT
	#define GLFRAMEBUFFERTEXTURE    glFramebufferTexture2DEXT
    #define GLGENRENDERBUFFERS      glGenRenderbuffersEXT
    #define GLBINDRENDERBUFFER      glBindRenderbufferEXT
    #define GLRENDERBUFFER          GL_RENDERBUFFER_EXT
	#define GLDEPTHCOMPONENT		GL_DEPTH_COMPONENT
    #define GLRENDERBUFFERSTORAGEMULTISAMPLE glRenderbufferStorageMultisampleEXT
    #define GLRENDERBUFFERSTORAGE            glRenderbufferStorageEXT
    #define GLFRAMEBUFFERRENDERBUFFER        glFramebufferRenderbufferEXT
    #define GLREADFRAMEBUFFER                GL_READ_FRAMEBUFFER_EXT
    #define GLDRAWFRAMEBUFFER                GL_DRAW_FRAMEBUFFER_EXT
#endif

#define GLENABLE			glEnable
#define GLDISABLE			glDisable
#define GLSCISSOR			glScissor

#define GLCOLORMASK			glColorMask
#define GLBLENDFUNC			glBlendFunc
#define GLBLENDEQUATION		glBlendEquation
#define GLDEPTHMASK			glDepthMask
#define GLDEPTHFUNC			glDepthFunc
#define GLALPHAFUNC			glAlphaFunc
#define GLPOLYGONMODE		glPolygonMode
#define GLHINT				glHint
#define GLCULLFACE			glCullFace
#define GLLINEWIDTH			glLineWidth

#define GLCLEARCOLOR		glClearColor
#if PLATFORM != PLATFORM_IOS && PLATFORM != PLATFORM_ANDROID
	#define GLCLEARDEPTH	glClearDepth
#else
	#define GLCLEARDEPTH	glClearDepthf
#endif

#define GLCLEARSTENCIL		glClearStencil
#define GLCLEAR				glClear

#define GLGETINTEGERV		glGetIntegerv
#define GLVIEWPORT			glViewport

#define GLDRAWARRAYS		glDrawArrays
#define GLDRAWELEMENTS		glDrawElements


#define GLGENBUFFERS		glGenBuffers
#define GLBINDBUFFER		glBindBuffer
#define GLBUFFERDATA		glBufferData
#define GLDELETEBUFFERS		glDeleteBuffers
#define GLVERTEXATTRIBPOINTER		glVertexAttribPointer
#define GLENABLEVERTEXATTRIBARRAY	glEnableVertexAttribArray
#define GLDISABLEVERTEXATTRIBARRAY  glDisableVertexAttribArray

#define GLSHADERSOURCE				glShaderSource
#define GLCREATESHADER				glCreateShader
#define GLCOMPILESHADER				glCompileShader
#define GLGETSHADERIV				glGetShaderiv
#define GLGETPROGRAMIV				glGetProgramiv
#define GLCREATEPROGRAM				glCreateProgram
#define GLATTACHSHADER				glAttachShader
#define GLLINKPROGRAM				glLinkProgram
#define GLUSEPROGRAM				glUseProgram
#define GLGETSHADERINFOLOG			glGetShaderInfoLog
#define GLGETPROGRAMINFOLOG			glGetProgramInfoLog
#define GLDELETEPROGRAM				glDeleteProgram
#define GLDELETESHADER				glDeleteShader


#define GLGETACTIVEUNIFORM			glGetActiveUniform
#define GLGETUNIFORMLOCATION		glGetUniformLocation
#define GLBINDATTRIBLOCATION		glBindAttribLocation

#define GLGENTEXTURES				glGenTextures
#define GLACTIVETEXTURE				glActiveTexture
#define GLBINDTEXTURE				glBindTexture
#define GLDELETETEXTURES			glDeleteTextures

#define GLUNIFORM1I					glUniform1i
#define GLUNIFORM1FV				glUniform1fv
#define GLUNIFORM2FV				glUniform2fv
#define GLUNIFORM3FV				glUniform3fv
#define GLUNIFORM4FV				glUniform4fv
#define GLUNIFORMMATRIX4FV			glUniformMatrix4fv

#define GLTEXPARAMETERI				glTexParameteri
#define GLTEXPARAMETERF				glTexParameterf

#define GLTEXIMAGE2D				glTexImage2D
#define GLTEXSUBIMAGE2D				glTexSubImage2D

#define GLCOPYTEXIMAGE2D			glCopyTexImage2D

#define GLCOMPRESSEDTEXIMAGE2D		glCompressedTexImage2D
#define GLCOMPRESSEDTEXSUBIMAGE2D	glCompressedTexSubImage2D

#if PLATFORM != PLATFORM_IOS && PLATFORM != PLATFORM_ANDROID
	#define GLTEXIMAGE3D                glTexImage3D
	#define GLTEXSUBIMAGE3D             glTexSubImage3D
	#define GLCOMPRESSEDTEXIMAGE3D      glCompressedTexImage3D
	#define GLCOMPRESSEDTEXSUBIMAGE3D	glCompressedTexSubImage3D
#endif


// constant macro convert table
extern GLenum g_BlendFunc[cBlendFuncTypeCount];

extern GLenum g_CMPFunc[cCmpFuncCount];

extern GLenum g_BlendFactor[cBlendFactorCount];

#if PLATFORM != PLATFORM_IOS && PLATFORM != PLATFORM_ANDROID
extern GLenum g_Fillmode[cFillModeCount];
#endif

extern GLenum g_AccessFlag[cLockFlagCount];

extern GLenum g_PrimTopology[cPrimitiveTypeCount];

extern GLint g_FallbackPath[cPixelFormatCount][8];

extern GLenum g_Usage[cUsageTypeCount];

inline void TestGLError()
{
#ifdef DEBUG
	GLenum error = glGetError();
	assert(error == GL_NO_ERROR);
#endif
}


