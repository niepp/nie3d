#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogltexture.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"
#include "meshresource.h"
#include "resourcecache.h"
#include "rendercontext.h"
#include "utilityrenderer.h"
#include "scenenode.h"
#include "lightnode.h"
#include "cameranode.h"
#include "postprocess.h"
#include "postprocessdof.h"
#include "scene.h"
#include "rendernode.h"
#include "meshnode.h"

MeshNode::MeshNode(MeshResource *mesh_res) : m_mesh_res(mesh_res)
{
	// temp code
	m_mtl = (Material*)ResourceCache::Instance()->Create("mesh.mtl", cResTypeMaterial);

}

void MeshNode::Update()
{

}

void MeshNode::FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env)
{
	Matrix to_world = GetLocalToWorldMatrix();
	Matrix to_local = GetWorldToLocalMatrix();
	MeshVec &meshes = m_mesh_res->GetMeshes();
	MeshVec::iterator itr = meshes.begin();
	for (; itr != meshes.end(); ++itr) 
	{
		RenderContext *rc = new RenderContext;
		rc->world_to_local = to_local;
		rc->local_to_world = to_world;
		rc->mvp = to_world * env->vp;
		rc->layout = m_mesh_res->GetMeshVertexLayout();
		rc->vb = itr->m_vb;
		rc->ib = itr->m_ib;
		rc->mtl = m_mtl;
		rc->prim_type = cTriangleList;
		rc->prim_vert_count = itr->index_count;
		rc->start_index = 0;
		rc->rs.cull_mode = cCullCCW;
		rc->rs.fill_mode = cFillSolid;

		PointLight *pl = (m_lights.size() > 0 && m_lights[0]->GetLightType() == cLightPoint) ? (PointLight*)m_lights[0] : NULL;
		if (pl != NULL) {
			const Vector3 &p = pl->GetPosition();
			const Color &c = pl->GetLightColor();
			rc->light_param.lightPositionRange.Set(p.x, p.y, p.z, pl->GetRange());
			rc->light_param.lightColorAtten.Set(c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, pl->GetAttenuation());
		}

		if (m_mtl) {
			Variant var;
			m_mtl->GetMtlParam("specularGlossy", var);
			rc->gbufer_constant.normalSpecularParam.x = var.GetFloat();
		}
		rc_queue->Append(rc);
	}

}

void MeshNode::RenderDebug(UtilityRenderer *renderer)
{
	// show normal/tangent etc
	//Matrix to_world = GetLocalToWorldMatrix();
	//std::vector<MeshData*> pMeshes;
	//m_mesh_res->GetMeshData(pMeshes);
	//for (Uint32 i = 0; i < pMeshes.size(); ++i)
	//{
	//	std::vector<MeshVertex> &vertex_data = pMeshes[i]->vertex_data;
	//	for (Uint32 j = 0; j < vertex_data.size(); ++j)
	//	{
	//		Vector3 p = vertex_data[j].position * to_world;
	//		Vector3 q = (vertex_data[j].position + vertex_data[j].normal * 0.1f) * to_world;
	//		renderer->DrawLine(p, q);

	//		Vector3 s = vertex_data[j].position * to_world;
	//		Vector3 t(vertex_data[j].tangent.x, vertex_data[j].tangent.y, vertex_data[j].tangent.z);
	//		t *= 0.1f;
	//		t += vertex_data[j].position;
	//		t = t * to_world;
	//		renderer->DrawLine(s, t, Color::Blue);
	//	}
	//}

}
