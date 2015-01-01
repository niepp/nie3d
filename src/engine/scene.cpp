#include "core/core.h"
#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"
#include "meshresource.h"
#include "resourcecache.h"
#include "material.h"
#include "rendercontext.h"
#include "utilityrenderer.h"
#include "scenenode.h"
#include "cameranode.h"
#include "lightnode.h"
#include "rendernode.h"
#include "postprocess.h"
#include "postprocessdof.h"
#include "scene.h"

Scene::Scene(IRenderer *renderer)
{
	m_rc_queue = new RenderContextQueue(renderer);

	m_screen_target = renderer->GetMainTarget();
	m_screen_target->SetClearParam(cClearZBuffer | cClearTarget, Color::Black, 1.0f, 0);

	m_utility_renderer = new UtilityRenderer(renderer);

	Uint32 w = 0, h = 0;
	m_screen_target->GetSize(w, h);

	TextureDesc texdesc;
	texdesc.width = w;
	texdesc.height = h;
	texdesc.format = cPixelFormatR8G8B8A8;
	texdesc.filepath[0] = 0;
	texdesc.depth = 32;
	texdesc.type = cTexture2D;
	texdesc.usage = cUsageTexture;
	texdesc.miplevels = 1;
	m_gbuffer_texture = renderer->CreateTexture(texdesc, NULL);
	m_gbuffer_texture->SetAddressMode(cAddressClamp);
	m_scene_texture = renderer->CreateTexture(texdesc, NULL);
	m_scene_texture->SetAddressMode(cAddressClamp);

	TextureDesc depthdesc;
	depthdesc.width = w;
	depthdesc.height = h;
	depthdesc.format = cPixelFormatR32_Float;
	depthdesc.filepath[0] = 0;
	depthdesc.depth = 32;
	depthdesc.type = cTexture2D;
	depthdesc.usage = cUsageDepthStencil;
	depthdesc.miplevels = 1;
	m_depth_texture = renderer->CreateTexture(depthdesc, NULL);
    m_depth_texture->SetAddressMode(cAddressClamp);

	IMaterialTemplate *mtltemp = renderer->CreateMaterialTemplate("deferredlighting.fx");
	ShaderMacro macro;
	m_gbuffer_shader = mtltemp->GetShader(macro, "gbuffer");
	m_gbuffer_target = renderer->CreateRenderTarget(w, h);
	m_gbuffer_target->SetClearParam(cClearZBuffer | cClearTarget, Color::Black, 1.0f, 0);
	m_gbuffer_target->AttachTexture(m_depth_texture, 0, m_gbuffer_texture, 0);

	m_light_target = renderer->CreateRenderTarget(w, h);
	m_light_texture = renderer->CreateTexture(texdesc, NULL);
	m_light_texture->SetAddressMode(cAddressClamp);
	m_light_target->SetClearParam(cClearTarget, Color::Zero, 1.0f, 0);
	m_light_target->AttachTexture(NULL, 0, m_light_texture, 0);

	IMaterialTemplate *light_mtl = renderer->CreateMaterialTemplate("lightshape.fx");
	m_light_shader = light_mtl->GetShader(macro, "light");

	m_scene_target = renderer->CreateRenderTarget(w, h);
	m_scene_target->SetClearParam(cClearZBuffer | cClearTarget, Color::Black, 1.0f, 0);
	m_scene_target->AttachTexture(NULL, 0, m_scene_texture, 0);

	// point light shape
	m_unit_sphere = new Sphere(5);

	InputElement elements0[] = {
		{0, 0, cInputUsagePosition, 0, cPixelFormatR32G32B32_Float},
		{0, 12, cInputUsageNormal,  0, cPixelFormatR32G32B32_Float},
	};

	m_point_shape_layout = renderer->CreateInputLayout(elements0, ArraySize(elements0));

	struct _ShapeVertex {
		Vector3 pos;
		Vector3 nor;
	};

	Uint32 v_count = m_unit_sphere->m_sphere_pos.size();
	Uint32 idx_count = m_unit_sphere->m_sphere_indices.size();
	_ShapeVertex *vb = new _ShapeVertex[v_count];
	for (Uint32 i = 0; i < v_count; ++i) {
		vb[i].pos = m_unit_sphere->m_sphere_pos[i];
		vb[i].nor = m_unit_sphere->m_sphere_nor[i];
	}

	m_point_shape_vb = renderer->CreateVertexBuffer(cUsageDynamic, v_count * sizeof(_ShapeVertex), sizeof(_ShapeVertex));
	m_point_shape_ib = renderer->CreateIndexBuffer(cUsageDynamic, idx_count, cIndex16);
	m_point_shape_vb->Update(vb, v_count * sizeof(_ShapeVertex));
	m_point_shape_ib->Update(&m_unit_sphere->m_sphere_indices[0], idx_count * sizeof(Uint16));

	// post process
	IMaterialTemplate *blit_mtl = renderer->CreateMaterialTemplate("blit.fx");
	m_blit_shader = blit_mtl->GetShader(macro, "blit");

	m_quad_render = new QuadRender;
	m_dof = new PostProcessDOF(m_depth_texture);

}

void Scene::AddNode(SceneNode *node)
{
	m_nodes.push_back(node);
	node->SetScene(this);
}

void Scene::RemoveNode(SceneNode *node)
{
	SceneNodeVec::iterator itr = m_nodes.begin();
	for (; itr != m_nodes.end(); ++itr)
	{
		if (*itr == node) {
			m_nodes.erase(itr);
			node->SetScene(NULL);
			break;
		}
	}
}

void Scene::Update()
{
	m_camera_node->Update();

	const Matrix &view = m_camera_node->GetViewMatrix();
	const Matrix &proj = m_camera_node->GetProjMatrix();
	const Vector3 &pos = m_camera_node->GetPosition();
	m_render_env.eye_pos = Vector4(pos.x, pos.y, pos.z, 1.0f);
	m_render_env.view = view;
	m_render_env.proj = proj;
	m_render_env.vp = view * proj;
	m_render_env.inv_vp = m_render_env.vp;
	m_render_env.inv_vp.Inverse();
	m_render_env.light_tex = m_light_texture;

	SceneNodeVec::iterator itr = m_nodes.begin();
	for (; itr != m_nodes.end(); ++itr)
	{
		(*itr)->Update();
	/*	RenderNode *renderNode = dynamic_cast<RenderNode*>(*itr);
		if (renderNode != NULL) {
			renderNode->ClearLight();
			LightVec::iterator i = m_lights.begin();
			for (; i != m_lights.end(); ++i)
			{
				if ((*i)->GetLightType() == cLightPoint) {
					PointLight *pl = (PointLight*)(*i);
					Float distSqur = (renderNode->GetPosition() - pl->GetPosition()).LengthSqr();
					Float range = pl->GetRange();
					if (distSqur <= range * range) {
						renderNode->AddLight(pl);
					}
				}
			}
		}*/
	}
}

void Scene::Render()
{
	m_rc_queue->Clear();

	for (SceneNodeVec::iterator itr = m_nodes.begin();
		itr != m_nodes.end(); ++itr)
	{
		if (Config::Instance().enable_debug_draw) {
			(*itr)->RenderDebug(m_utility_renderer);
		}
		(*itr)->FillRenderContext(m_rc_queue, &m_render_env);
	}

	for (LightVec::iterator itr = m_lights.begin();
		itr != m_lights.end(); ++itr)
	{
		if (Config::Instance().enable_debug_draw) {
			(*itr)->RenderDebug(m_utility_renderer);
		}
	}

	// gbuffer pass
	m_gbuffer_target->BeginScene();
	m_rc_queue->DrawWithShader(m_gbuffer_shader, &m_render_env);
	m_gbuffer_target->EndScene();

	//TextureDesc tex_desc;
	//m_gbuffer_texture->GetDesc(tex_desc);

	//IRenderTarget::LockResult res;
	//m_gbuffer_target->Lock(res);
	//SaveTextureBMP(tex_desc, (const Uint8*)res.bits, "gbuf.bmp");
	//m_gbuffer_target->Unlock();
	

	// light pass
	GetRenderer()->RestoreDefaultState();
	m_light_target->BeginScene();
	for (LightVec::iterator itr = m_lights.begin();
		itr != m_lights.end(); ++itr)
	{
		RenderLight(*itr);
	}
	m_light_target->EndScene();

	// fainl pass
	m_scene_target->BeginScene();
	m_rc_queue->DoDraw(&m_render_env);
	m_scene_target->EndScene();

//	ITexture *dst = m_dof->Blit(m_scene_texture);

	m_screen_target->BeginScene();
	m_quad_render->DrawQuad(m_scene_texture, m_blit_shader);

	if (Config::Instance().enable_debug_draw) {
		m_utility_renderer->DoDraw(&m_render_env);
	}

	m_screen_target->EndScene();
	m_screen_target->Present();
}

void Scene::RenderLight(LightNode *light)
{
	IRenderer *renderer = GetRenderer();

	Matrix to_world = light->GetLocalToWorldMatrix();
	Matrix to_local = light->GetWorldToLocalMatrix();

	RenderContext rc;
	rc.world_to_local = to_local;
	rc.local_to_world = to_world;
	rc.mvp = to_world * m_render_env.vp;
	rc.layout = m_point_shape_layout;
	rc.vb = m_point_shape_vb;
	rc.ib = m_point_shape_ib;
	rc.mtl = NULL;
	rc.prim_type = cTriangleList;
	rc.prim_vert_count = m_unit_sphere->m_sphere_indices.size();
	rc.start_index = 0;
	rc.rs.cull_mode = cCullCCW;
	rc.rs.fill_mode = cFillSolid;
	rc.rs.SetRenderState(cBlendEnable, true);
	rc.rs.SetRenderState(cBlendFunc, cBlendFuncAdd);
	rc.rs.SetRenderState(cSrcBlend, cBlendOne);
	rc.rs.SetRenderState(cDestBlend, cBlendOne);
	rc.rs.SetRenderState(cDepthTestEnable, 0);
	//rc.rs.SetRenderState(cDepthWriteEnable, 0);

	PointLight *pl = (light->GetLightType() == cLightPoint) ? (PointLight*)light : NULL;
	if (pl != NULL) {
		const Vector3 &p = pl->GetPosition();
		const Color &c = pl->GetLightColor();
		Float range = pl->GetRange();
		rc.light_param.lightPositionRange.Set(p.x, p.y, p.z, range);
		rc.light_param.lightColorAtten.Set(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, pl->GetAttenuation());

		Vector3 cameraToLight = p - GetMainCamera()->GetPosition();
		if (cameraToLight.LengthSqr() < range * range) {
			rc.rs.cull_mode = cCullCW;
		}
		else {
			rc.rs.cull_mode = cCullCCW;
		}
	}

	m_light_shader->BeginPass(0);
	m_light_shader->SetConstant("g_Eye", &m_render_env.eye_pos);
	m_light_shader->SetConstant("g_WorldViewProj", &rc.mvp);
	m_light_shader->SetConstant("g_LocalToWorld", &rc.local_to_world);
	m_light_shader->SetConstant("g_WorldToLocal", &rc.world_to_local);
	m_light_shader->SetConstant("g_ViewProj", &m_render_env.vp);
	m_light_shader->SetConstant("g_InverseViewProj", &m_render_env.inv_vp);
	m_light_shader->SetConstant("g_PointlightPositionRange", &rc.light_param.lightPositionRange);
	m_light_shader->SetConstant("g_PointlightColorAtten", &rc.light_param.lightColorAtten);
	m_light_shader->SetTexture("_DepthTex", m_depth_texture);
	m_light_shader->SetTexture("_GBufferTex", m_gbuffer_texture);

	renderer->SetRenderState(rc.rs);
	renderer->SetVertexBuffer(rc.vb);
	renderer->SetInputLayout(rc.layout);
	renderer->Draw(rc.prim_type, rc.prim_vert_count, rc.ib, rc.start_index);
	m_light_shader->EndPass();

}

void Scene::SetMainCamera(CameraNode *camera)
{
	m_camera_node = camera;
}

CameraNode* Scene::GetMainCamera()
{
	return m_camera_node;
}

void Scene::AddLight(LightNode *light)
{
	LightVec::iterator itr = m_lights.begin();
	for (; itr != m_lights.end(); ++itr)
	{
		if (*itr == light) {
			break;
		}
	}
	if (itr == m_lights.end()) {
		m_lights.push_back(light);
	}
}

void Scene::DelLight(LightNode *light)
{
	LightVec::iterator itr = m_lights.begin();
	for (; itr != m_lights.end(); ++itr)
	{
		if (*itr == light) {
			m_lights.erase(itr);
			break;
		}
	}
}
