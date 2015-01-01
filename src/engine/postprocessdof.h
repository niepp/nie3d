#pragma once

#ifndef __POST_PROCESS_DOF_H__
#define __POST_PROCESS_DOF_H__

class PostProcessDOF : public PostProcess {
public:
	PostProcessDOF(ITexture *depth_tex);
	virtual ~PostProcessDOF();
	virtual ITexture* Blit(ITexture *src);
private:
	Vector4 m_tex_size;
	ITexture *m_depth_tex;
	ITexture *m_blur_tex;
	IRenderTarget *m_blur_target;
	Vector4 m_paramter;
};

#endif // __POST_PROCESS_DOF_H__