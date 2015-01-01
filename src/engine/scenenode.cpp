#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogltexture.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"
#include "meshresource.h"
#include "rendercontext.h"
#include "utilityrenderer.h"
#include "scenenode.h"
#include "cameranode.h"
#include "lightnode.h"
#include "postprocess.h"
#include "postprocessdof.h"
#include "scene.h"

SceneNode::SceneNode() : m_pos(0,0,0), m_rot(0,0,0,1), m_scale(1.0f, 1.0f, 1.0f)
	, m_mat_dirty(true)
{

}

void SceneNode::Update()
{

}

void SceneNode::FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env)
{

}

void SceneNode::RenderDebug(UtilityRenderer *renderer)
{

}

Scene* SceneNode::GetScene()
{
	return m_scene;
}

void SceneNode::SetScene(Scene *scene)
{
	m_scene = scene;
}

void SceneNode::SetPosition(const Vector3& pos)
{
	m_pos = pos;
	m_mat_dirty = true;
}

void SceneNode::SetRotation(const Quaternion& rot)
{
	m_rot = rot;
	m_mat_dirty = true;
}

void SceneNode::SetScale(const Vector3& scale)
{
	m_scale = scale;
	m_mat_dirty = true;
}

const Vector3& SceneNode::GetPosition() const
{
	return m_pos;
}

const Quaternion& SceneNode::GetRotation() const
{
	return m_rot;
}

const Vector3& SceneNode::GetScale() const
{
	return m_scale;
}

const Matrix& SceneNode::GetWorldToLocalMatrix()
{
	UpdateMatrix();
	return m_world2local_mat;
}

const Matrix& SceneNode::GetLocalToWorldMatrix()
{
	UpdateMatrix();
	return m_local2world_mat;
}

void SceneNode::UpdateMatrix()
{
	if (m_mat_dirty) {
		m_mat_dirty = false;
		m_local2world_mat.Compose(m_pos, m_rot, m_scale);
		m_world2local_mat = m_local2world_mat;
		m_world2local_mat.Inverse();
	}
}
