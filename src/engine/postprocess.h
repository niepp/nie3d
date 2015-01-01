#pragma once

#ifndef __POST_PROCESS_H__
#define __POST_PROCESS_H__

class QuadRender {
public:
	QuadRender();
	~QuadRender();
	void DrawQuad(ITexture *src, IShader *shader);
private:
	IInputLayout *m_quad_layout;
	IVertexBuffer *m_quad_vb;
};

class  PostProcess {
public:
	PostProcess() : m_rt(NULL), m_shader(NULL) { }
	virtual ITexture* Blit(ITexture *src) = 0;
protected:
	IRenderTarget *m_rt;
	ITexture *m_dst;
	IShader *m_shader;
	QuadRender m_quad_render;

};

#endif // __POST_PROCESS_H__