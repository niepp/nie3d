#pragma once

class OGLInputLayout : public IInputLayout
{
public:
	OGLInputLayout(const InputElement *pelements, Uint32 nelement);
	~OGLInputLayout();
    void Begin();
    void End();
	IVertexArrayObject* CreateVAO(IVertexBuffer *pvb, IIndexBuffer *pib);
public:
	std::vector<InputElement> m_ElementArray;
	Uint32 m_nElement;
	Uint32 m_stride;
	
};

class OGLVertexArrayObject : public IVertexArrayObject
{
public:
	virtual ~OGLVertexArrayObject();

public:
	OGLVertexBuffer *m_vb;
	OGLIndexBuffer *m_ib;
	GLuint m_vao;

};
