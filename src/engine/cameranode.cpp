#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogltexture.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"
#include "rendercontext.h"

#include "utilityrenderer.h"
#include "scenenode.h"
#include "cameranode.h"

CameraNode::CameraNode() : m_fov(45.0f)
		, m_aspect(16.0f / 9.0f)
		, m_znear(1.0f)
		, m_zfar(1000.0f)
{

}

void CameraNode::Update()
{
	Vector3 right, up, forward;
	m_rot.GetRight(right);
	m_rot.GetUp(up);
	m_rot.GetForward(forward);

	m_view_mat.MakeMatViewR(m_pos, m_pos + forward, up);
	m_proj_mat.MakeMatPerspR(DegreeToRadian(m_fov), m_aspect, m_znear, m_zfar);
}

void CameraNode::FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env)
{

}

void CameraNode::RenderDebug(UtilityRenderer *renderer)
{

}
