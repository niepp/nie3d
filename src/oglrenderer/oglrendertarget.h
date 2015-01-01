#pragma once

class OGLRenderTarget : public IRenderTarget
{
public:
	OGLRenderTarget(Uint32 width, Uint32 height);
	virtual ~OGLRenderTarget();
	virtual Bool BeginScene(Uint32 index = 0);
	virtual void EndScene(Uint32 index = 0);
	virtual void Present();
	virtual void AttachTexture(ITexture *depth_tex, Uint32 depth_level,	ITexture *rt_tex, Uint32 rt_level);

	virtual Bool Lock(LockResult& outLesult, const RectInt* inRect = NULL);
	virtual void Unlock();
	virtual void GetSize(Uint32& fWidth, Uint32& fHeight);

	virtual void SetClearParam(Uint32 flags, const Color &color, Float z, Uint32 stencil);
	virtual void HandlePossibleSizeChange();

public:
	GLuint m_FrameBuffer;
	GLuint m_DepthBuffer;

	Uint32	m_ClearFlags;
	Color	m_ClearColor;
	Float	m_ClearDepth;
	Uint32	m_ClearStencil;

	Uint32 m_Width;
	Uint32 m_Height;

private:
	GLint m_originViewPort[4];
	Int8* m_pInternalLockBuffer;

};
