#include "core/core.h"

#include "ogl.h"
#include "ogldefine.h"
#include "oglgeobuffer.h"

OGLVertexBuffer::OGLVertexBuffer(Uint32 buffersize, UsageType usage, Uint32 strideInBytes)
	:m_BufferSize(buffersize)
    ,m_StrideInBytes(strideInBytes)
{
	m_BufferUsage = g_Usage[usage];

	GLGENBUFFERS(1,&m_VertexBuffer);
	GLBINDBUFFER(GL_ARRAY_BUFFER,m_VertexBuffer);
	GLBUFFERDATA(GL_ARRAY_BUFFER,m_BufferSize,NULL,m_BufferUsage);

}

void OGLVertexBuffer::Update(const void* pdata, Uint32 size)
{
    m_BufferSize = size;
	GLBINDBUFFER(GL_ARRAY_BUFFER,m_VertexBuffer);
    GLBUFFERDATA(GL_ARRAY_BUFFER,m_BufferSize,pdata,m_BufferUsage);
}

void* OGLVertexBuffer::Lock(Uint32 lockFlags)
{
    GLBINDBUFFER(GL_ARRAY_BUFFER,m_VertexBuffer);
    void* pdata = GLMAPBUFFER(GL_ARRAY_BUFFER, g_AccessFlag[lockFlags]);
    m_bLocked = true;
    return pdata;
}

void OGLVertexBuffer::Unlock()
{
    GLBINDBUFFER(GL_ARRAY_BUFFER,m_VertexBuffer);
    GLUNMAPBUFFER(GL_ARRAY_BUFFER);
    m_bLocked = false;	
}

Bool  OGLVertexBuffer::IsLocked()
{
	return m_bLocked;
}

Uint32 OGLVertexBuffer::GetNumOfVertexes()
{
    assert((m_BufferSize % m_StrideInBytes) == 0);
	return m_BufferSize / m_StrideInBytes;
}


void OGLVertexBuffer::DeleteThis()
{
    GLDELETEBUFFERS(1, &m_VertexBuffer);
    m_VertexBuffer = 0;
}

OGLIndexBuffer::OGLIndexBuffer(Uint32 buffersize, UsageType usage)
	:m_BufferSize(buffersize)
{
	m_BufferUsage = g_Usage[usage];
	GLGENBUFFERS(1,&m_IndexBuffer);
	GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);
	GLBUFFERDATA(GL_ELEMENT_ARRAY_BUFFER,m_BufferSize,NULL,m_BufferUsage);
}

void* OGLIndexBuffer::Lock(Uint32 flags)
{
    GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);
    void* pdata = GLMAPBUFFER(GL_ELEMENT_ARRAY_BUFFER, g_AccessFlag[flags]);
    m_bLocked = true;
	return pdata;
}

void  OGLIndexBuffer::Unlock()
{
    GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);
    GLUNMAPBUFFER(GL_ELEMENT_ARRAY_BUFFER);	
    m_bLocked = false;			
}

void OGLIndexBuffer::Update(const void* pdata, Uint32 size)
{
    m_BufferSize = size;
	GLBINDBUFFER(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);
	GLBUFFERDATA(GL_ELEMENT_ARRAY_BUFFER,m_BufferSize,pdata,m_BufferUsage);
}

Bool  OGLIndexBuffer::IsLocked()
{
	return m_bLocked;
}

void OGLIndexBuffer::SetVertexRange(Uint32 startvert, Uint32 nvert)
{
	
}

void OGLIndexBuffer::DeleteThis()
{
    GLDELETEBUFFERS(1, &m_IndexBuffer);
    m_IndexBuffer = 0;
}
