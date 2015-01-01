#pragma once

class ENGINE_API CameraNode : public SceneNode {
public:
	CameraNode();
	virtual void Update();
	virtual void FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env);
	virtual void RenderDebug(UtilityRenderer *renderer);
	const Matrix& GetViewMatrix() { return m_view_mat; }
	const Matrix& GetProjMatrix() { return m_proj_mat; }
	Float GetFov() const { return m_fov; }
	Float GetAspect() const { return m_aspect; }
	void SetFov(Float fov) { m_fov = fov; }
	void SetAspect(Float aspect) { m_aspect = aspect; }

private:
	Matrix m_view_mat;
	Matrix m_proj_mat;
	Float m_fov;
	Float m_aspect;
	Float m_znear;
	Float m_zfar;
};
