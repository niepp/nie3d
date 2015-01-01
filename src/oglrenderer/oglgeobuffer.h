#pragma once

class OGLVertexBuffer : public IVertexBuffer
{
public:
	OGLVertexBuffer(Uint32 buffersize, UsageType usage, Uint32 strideInBytes);

	virtual void* Lock(Uint32 lockFlags);
	virtual void Unlock();
	virtual void Update(const void* pdata, Uint32 size = 0);
	virtual Bool IsLocked();

	virtual Uint32 GetNumOfVertexes();

protected:
	virtual void DeleteThis();
	
public:
	GLuint  m_VertexBuffer;
	GLenum  m_BufferUsage;
	GLuint  m_BufferSize;
    Uint32  m_StrideInBytes;
	void*	m_pVertexData;
    Bool    m_bLocked;
};

class OGLIndexBuffer : public IIndexBuffer
{

public:
	OGLIndexBuffer(Uint32 buffersize, UsageType usage);

	virtual void *Lock(Uint32 flags);
	virtual void Unlock();
	virtual void Update(const void* pdata, Uint32 size = 0);
	virtual Bool  IsLocked();

	virtual void SetVertexRange(Uint32 startvert, Uint32 nvert);

protected:
	virtual void DeleteThis();

public:
	GLuint m_IndexBuffer;
	GLenum m_BufferUsage;
    GLuint m_BufferSize;
    Bool m_bLocked;
};
