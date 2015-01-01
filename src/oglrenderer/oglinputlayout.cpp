#include "core/core.h"
#include "ogl.h"
#include "ogldefine.h"
#include "oglgeobuffer.h"
#include "oglinputlayout.h"

const Uint32 FormatSize[cPixelFormatCount] =
{
	0,
	2,
	2,
	2,
	3,
	4,
	4,
	4,
	4,
	4,
	1,
	4,
	2,
	2,
	4,
	4,
	8,
	8,
	8,
	4,
	12,
	16,
	0,
	0,
	0,
	2,
	4,
	2,
	4,
	4,
	4,
	2,
	4,
	4,
	8,
	1,
	8
};

inline void GetElementGLFormat(const PixelFormat &format, GLint &size, GLenum &type)
{
	switch(format)	
	{
	case cPixelFormatR32G32B32_Float:
		size = 3;
		type = GL_FLOAT;
		break;
	case cPixelFormatR32G32B32A32_Float:
		size = 4;
		type = GL_FLOAT;
		break;
	case cPixelFormatR32G32_Float:
		size = 2;
		type = GL_FLOAT;
		break;
	case cPixelFormatR8G8B8A8:
	case cPixelFormatR8G8B8A8_Unorm:
	case cPixelFormatR8G8B8A8_UInt:
	case cPixelFormatR8G8B8A8_SNorm:
	case cPixelFormatR8G8B8A8_SInt:
		size = 4;
		type = GL_UNSIGNED_BYTE;
		break;
    case cPixelFormatR16G16_UInt:
        size = 2;
        type = GL_UNSIGNED_SHORT;
        break;
	default:
		assert(0);
	}
}

inline GLint ComputeVertexAttributeIndex(const InputElement &element)
{
	return element.usage + element.index * (cInputUsageCount - cInputUsageTexcoord);
}

OGLInputLayout::OGLInputLayout(const InputElement *pelements, Uint32 nelement)
{
	for(Uint32 i = 0; i < nelement; ++i)
	{
		m_ElementArray.push_back(pelements[i]);
	}

	m_nElement = nelement;
	m_stride = pelements[nelement - 1].offset + FormatSize[pelements[nelement - 1].format];

}

OGLInputLayout::~OGLInputLayout()
{
	
}

void OGLInputLayout::Begin()
{
	for(Uint32 i = 0; i < m_nElement; ++i)
	{
		InputElement &element = m_ElementArray[i];
	    GLint index = ComputeVertexAttributeIndex(element);
		GLENABLEVERTEXATTRIBARRAY(index);

		GLint size = 0;
		GLenum type = 0;
		GetElementGLFormat(element.format, size, type);
		// void glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
		// pointer 这个参数是个多面手。如果没有使用 VBO，它指向 CPU 内存中的顶点数据数组；如果使用 VBO 绑定到 GL_ARRAY_BUFFER，那么它表示该种类型顶点数据在顶点缓存中的起始偏移量。
		GLVERTEXATTRIBPOINTER(index, size, type, false, m_stride, (void*)element.offset);
	}
}

void OGLInputLayout::End()
{
    for(Uint32 i = 0; i < m_nElement; ++i)
    {
        InputElement &element = m_ElementArray[i];
        GLint index = ComputeVertexAttributeIndex(element);
        GLDISABLEVERTEXATTRIBARRAY(index);
    }
}

OGLVertexArrayObject::~OGLVertexArrayObject()
{
	GLDELETEVERTEXARRAYS(1, &m_vao);
}

IVertexArrayObject* OGLInputLayout::CreateVAO(IVertexBuffer *pvb, IIndexBuffer *pib)
{
	OGLVertexArrayObject *vao = new OGLVertexArrayObject;
	vao->m_vb = static_cast<OGLVertexBuffer*>(pvb);
	vao->m_ib = static_cast<OGLIndexBuffer*>(pib);

	GLGENVERTEXARRAYS(1, &vao->m_vao);
	GLBINDVERTEXARRAY(vao->m_vao);

	GLBINDBUFFER(GL_ARRAY_BUFFER, vao->m_vb->m_VertexBuffer);

	if (pib != NULL)
	{
		GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER, vao->m_ib->m_IndexBuffer);
	}

	for(Uint32 i = 0; i < m_nElement; ++i)
	{
		InputElement &element = m_ElementArray[i];
		GLint index = ComputeVertexAttributeIndex(element);
		GLENABLEVERTEXATTRIBARRAY(index);

		GLint size(0);
		GLenum type(0);
		GetElementGLFormat(element.format, size, type);
		GLVERTEXATTRIBPOINTER(index, size, type, false, m_stride, (void*)element.offset);

	}

	GLBINDVERTEXARRAY(0);
	GLBINDBUFFER(GL_ARRAY_BUFFER, 0);
	GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER, 0);

	return vao;

}
