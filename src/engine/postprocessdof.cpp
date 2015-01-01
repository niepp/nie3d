#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogltexture.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"
#include "meshresource.h"
#include "material.h"
#include "rendercontext.h"
#include "utilityrenderer.h"
#include "scenenode.h"
#include "cameranode.h"
#include "lightnode.h"
#include "rendernode.h"
#include "postprocess.h"
#include "postprocessdof.h"

PostProcessDOF::PostProcessDOF(ITexture *depth_tex) : m_depth_tex(depth_tex)
{
	IRenderer *renderer = GetRenderer();

	Uint32 w = 0, h = 0;
	renderer->GetMainTarget()->GetSize(w, h);

	TextureDesc texdesc;
	texdesc.width = w;
	texdesc.height = h;
	texdesc.format = cPixelFormatR8G8B8A8;
	texdesc.filepath[0] = 0;
	texdesc.depth = 32;
	texdesc.type = cTexture2D;
	texdesc.usage = cUsageTexture;
	texdesc.miplevels = 1;
	m_blur_tex = renderer->CreateTexture(texdesc, NULL);
	m_dst = renderer->CreateTexture(texdesc, NULL);

	m_blur_target = renderer->CreateRenderTarget(w, h);
	m_blur_target->AttachTexture(NULL, 0, m_blur_tex, 0);
	m_blur_target->SetClearParam(cClearZBuffer| cClearTarget, Color::Black, 1.0f, 0);

	m_rt = renderer->CreateRenderTarget(w, h);
	m_rt->AttachTexture(NULL, 0, m_dst, 0);
	m_rt->SetClearParam(cClearZBuffer| cClearTarget, Color::Black, 1.0f, 0);

	IMaterialTemplate *mtl_temp = renderer->CreateMaterialTemplate("dof.fx");
	ShaderMacro macro;
	m_shader = mtl_temp->GetShader(macro, "dof");
	m_paramter.Set(50.0f, 50.0f, 100.0f, 0);

}

PostProcessDOF::~PostProcessDOF()
{

}

ITexture* PostProcessDOF::Blit(ITexture *src)
{
	Uint32 width = 0, height = 0;
	src->GetSize(width, height);
	m_tex_size.Set(1.0f / (Float)width, 0, 0, 0);

	m_blur_target->BeginScene();
	m_shader->SetTexture("_DepthTex", m_depth_tex);
	m_shader->SetConstant("g_DOFParam", &m_paramter);
	m_shader->SetConstant("g_texSize", &m_tex_size);
	m_quad_render.DrawQuad(src, m_shader);
	m_blur_target->EndScene();

	m_tex_size.Set(0, 1.0f / (Float)height, 0, 0);

	m_rt->BeginScene();
	m_shader->SetTexture("_DepthTex", m_depth_tex);
	m_shader->SetConstant("g_DOFParam", &m_paramter);
	m_shader->SetConstant("g_texSize", &m_tex_size);
	m_quad_render.DrawQuad(m_blur_tex, m_shader);
	m_rt->EndScene();

	return m_dst;
}
