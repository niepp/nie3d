#include <algorithm>
#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"
#include "rendercontext.h"
#include "utilityrenderer.h"

UtilityRenderer::UtilityRenderer(IRenderer *renderer)
{
	IMaterialTemplate *mtltemp = renderer->CreateMaterialTemplate("line.fx");
	ShaderMacro macro;

	m_shader = mtltemp->GetShader(macro, "tech0");

	// render related
	InputElement elements0[] =
	{
		{0, 0, cInputUsagePosition, 0, cPixelFormatR32G32B32_Float},
		{0, 12, cInputUsageColor, 0, cPixelFormatR8G8B8A8_Unorm}
	};

	m_layout = renderer->CreateInputLayout(elements0, ArraySize(elements0));
	m_vb = renderer->CreateVertexBuffer(cUsageDynamic, 32 * sizeof(ColorVertex), sizeof(ColorVertex));
	m_vb_test_depth = renderer->CreateVertexBuffer(cUsageDynamic, 32 * sizeof(ColorVertex), sizeof(ColorVertex));
	m_sphere_vb = renderer->CreateVertexBuffer(cUsageDynamic, 32 * sizeof(ColorVertex), sizeof(ColorVertex));
	m_sphere_vb_test_depth = renderer->CreateVertexBuffer(cUsageDynamic, 32 * sizeof(ColorVertex), sizeof(ColorVertex));
	m_sphere_ib = renderer->CreateIndexBuffer(cUsageDynamic, 96, cIndex16);
	m_sphere_ib_test_depth = renderer->CreateIndexBuffer(cUsageDynamic, 96, cIndex16);
	m_unit_sphere = new Sphere(8);

}

UtilityRenderer::~UtilityRenderer()
{
	if (m_unit_sphere != NULL) {
		delete m_unit_sphere;
		m_unit_sphere = NULL;
	}
}

void UtilityRenderer::DrawLine(const Vector3 &pos0, const Vector3 &pos1, Color color, Bool zTestEnabled)
{
	ColorVertex v1, v2;
	v1.pos = pos0;
	v1.color = color;
	v2.pos = pos1;
	v2.color = color;
	if (zTestEnabled) {
		m_vertex_test_depth.push_back(v1);
		m_vertex_test_depth.push_back(v2);
	}
	else {
		m_vertex.push_back(v1);
		m_vertex.push_back(v2);
	}
}

void UtilityRenderer::DrawSphere(const Vector3 &center, Float radius, Color color, Bool zTestEnabled)
{
	Uint32 v_count = m_unit_sphere->m_sphere_pos.size();
	VertexVec v_vec(v_count);

	// transform
	for (Uint32 i = 0; i < v_count; ++i)
	{
		v_vec[i].pos = m_unit_sphere->m_sphere_pos[i] * radius + center;
		v_vec[i].color = m_unit_sphere->m_sphere_color[i];
	}

	if (zTestEnabled) {
		std::vector<Uint16> i_vec = m_unit_sphere->m_sphere_indices;
		for (std::vector<Uint16>::iterator it = i_vec.begin();
			it != i_vec.end(); ++it)
		{
			*it += m_sphere_vectex_test_depth.size();
		}
		m_sphere_indices_test_depth.insert(m_sphere_indices_test_depth.end(), i_vec.begin(), i_vec.end());
		m_sphere_vectex_test_depth.insert(m_sphere_vectex_test_depth.end(), v_vec.begin(), v_vec.end());
	}
	else {
		std::vector<Uint16> i_vec = m_unit_sphere->m_sphere_indices;
		for (std::vector<Uint16>::iterator it = i_vec.begin();
			it != i_vec.end(); ++it)
		{
			*it += m_sphere_vectex.size();
		}
		m_sphere_indices.insert(m_sphere_indices.end(), i_vec.begin(), i_vec.end());
		m_sphere_vectex.insert(m_sphere_vectex.end(), v_vec.begin(), v_vec.end());
	}
}

void UtilityRenderer::DoDraw(const RenderEnv *env)
{
	if (!m_shader) {
		return;
	}

	IRenderer *renderer = GetRenderer();
	renderer->RestoreDefaultState();
	if (m_vertex.size() > 0)
	{
		m_vb->Update(&m_vertex[0], m_vertex.size() * sizeof(ColorVertex));

		m_shader->BeginPass(0);
		m_shader->SetConstant("g_WorldViewProj", &env->vp);

		RenderState rs;
		rs.SetRenderState(cDepthTestEnable, 0);
//		rs.SetRenderState(cDepthWriteEnable, 0);
		renderer->SetRenderState(rs);
		renderer->SetVertexBuffer(m_vb);
		renderer->SetInputLayout(m_layout);
		renderer->Draw(cLineList, m_vertex.size());
		m_vertex.clear();
	}

	if (m_vertex_test_depth.size() > 0)
	{
		m_vb_test_depth->Update(&m_vertex_test_depth[0], m_vertex_test_depth.size() * sizeof(ColorVertex));

		m_shader->BeginPass(0);
		m_shader->SetConstant("g_WorldViewProj", &env->vp);

		RenderState rs;
		rs.SetRenderState(cDepthTestEnable, 1);
//		rs.SetRenderState(cDepthWriteEnable, 0);
		renderer->SetRenderState(rs);
		renderer->SetVertexBuffer(m_vb_test_depth);
		renderer->SetInputLayout(m_layout);
		renderer->Draw(cLineList, m_vertex_test_depth.size());
		m_vertex_test_depth.clear();
	}

	if (m_sphere_vectex.size() > 0)
	{
		m_sphere_vb->Update(&m_sphere_vectex[0], m_sphere_vectex.size() * sizeof(ColorVertex));
		m_sphere_ib->Update(&m_sphere_indices[0], m_sphere_indices.size() * sizeof(Uint16));

		m_shader->BeginPass(0);
		m_shader->SetConstant("g_WorldViewProj", &env->vp);

		RenderState rs;
		rs.SetRenderState(cDepthTestEnable, 0);
		//rs.SetRenderState(cDepthWriteEnable, 0);
		renderer->SetRenderState(rs);
		renderer->SetVertexBuffer(m_sphere_vb);
		renderer->SetInputLayout(m_layout);
		renderer->Draw(cTriangleList, m_sphere_indices.size(), m_sphere_ib, 0);
		m_sphere_vectex.clear();
		m_sphere_indices.clear();
	}

	if (m_sphere_vectex_test_depth.size() > 0)
	{
		m_sphere_vb_test_depth->Update(&m_sphere_vectex_test_depth[0], m_sphere_vectex_test_depth.size() * sizeof(ColorVertex));
		m_sphere_ib_test_depth->Update(&m_sphere_indices_test_depth[0], m_sphere_indices_test_depth.size() * sizeof(Uint16));

		m_shader->BeginPass(0);
		m_shader->SetConstant("g_WorldViewProj", &env->vp);

		RenderState rs;
		rs.SetRenderState(cDepthTestEnable, 1);
//		rs.SetRenderState(cDepthWriteEnable, 0);
		renderer->SetRenderState(rs);
		renderer->SetVertexBuffer(m_sphere_vb_test_depth);
		renderer->SetInputLayout(m_layout);
		renderer->Draw(cTriangleList, m_sphere_indices_test_depth.size(), m_sphere_ib_test_depth, 0);

		m_sphere_vectex_test_depth.clear();
		m_sphere_indices_test_depth.clear();
	}

}
