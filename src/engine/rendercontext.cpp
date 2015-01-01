#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogltexture.h"
#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"

#include "rendercontext.h"

void RenderContextQueue::Append(const RenderContext *rc)
{
	m_rc_vec.push_back(rc);
}

void RenderContextQueue::Clear()
{
	RenderContextVec::iterator itr = m_rc_vec.begin();
	for (; itr != m_rc_vec.end(); ++itr)
	{
		delete *itr;
		*itr = NULL;
	}
	m_rc_vec.clear();

}

void RenderContextQueue::DoDraw(RenderEnv *render_env)
{
	m_renderer->RestoreDefaultState();
	RenderContextVec::iterator itr = m_rc_vec.begin();
	for (; itr != m_rc_vec.end(); ++itr)
	{
		const RenderContext *rc = *itr;
		IShader *shader = NULL;
		if (rc->mtl) {
			shader = rc->mtl->GetRenderShader();
		}
		if (shader){
			shader->BeginPass(0);
			shader->SetConstant("g_WorldViewProj", &rc->mvp);
			shader->SetConstant("g_LocalToWorld", &rc->local_to_world);
			shader->SetConstant("g_WorldToLocal", &rc->world_to_local);
			shader->SetConstant("g_PointlightPositionRange", &rc->light_param.lightPositionRange);
			shader->SetConstant("g_PointlightColorAtten", &rc->light_param.lightColorAtten);
			shader->SetTexture("_LightTex", render_env->light_tex);
		}
		if (rc->mtl) {
			rc->mtl->SetParamsToShader(shader);
		}

		m_renderer->SetRenderState(rc->rs);
		m_renderer->SetVertexBuffer(rc->vb);
		m_renderer->SetInputLayout(rc->layout);
		m_renderer->Draw(rc->prim_type, rc->prim_vert_count, rc->ib, rc->start_index);
	}
}

void RenderContextQueue::DrawWithShader(IShader *shader, RenderEnv *render_env)
{
	assert(shader != NULL);
	m_renderer->RestoreDefaultState();
	RenderContextVec::iterator itr = m_rc_vec.begin();
	for (; itr != m_rc_vec.end(); ++itr)
	{
		const RenderContext *rc = *itr;
		shader->BeginPass(0);
		shader->SetConstant("g_WorldViewProj", &rc->mvp);
		shader->SetConstant("g_LocalToWorld", &rc->local_to_world);
		shader->SetConstant("g_WorldToLocal", &rc->world_to_local);
		shader->SetConstant("g_NormalSpecularParam", &rc->gbufer_constant.normalSpecularParam);
		shader->SetConstant("g_PointlightPositionRange", &rc->light_param.lightPositionRange);
		shader->SetConstant("g_PointlightColorAtten", &rc->light_param.lightColorAtten);

		m_renderer->SetRenderState(rc->rs);
		m_renderer->SetVertexBuffer(rc->vb);
		m_renderer->SetInputLayout(rc->layout);
		m_renderer->Draw(rc->prim_type, rc->prim_vert_count, rc->ib, rc->start_index);
		shader->EndPass();
	}
}
