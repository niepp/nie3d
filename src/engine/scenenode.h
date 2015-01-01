#pragma once
#include <vector>
#include <map>

class Scene;
class ENGINE_API SceneNode {
public:
	SceneNode();
	virtual void Update();
	virtual void FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env);
	virtual void RenderDebug(UtilityRenderer *renderer);
	Scene* GetScene();
	void SetScene(Scene *scene);
	void SetPosition(const Vector3& pos);
	void SetRotation(const Quaternion& rot);
	void SetScale(const Vector3& scale);
	const Vector3& GetPosition() const;
	const Quaternion& GetRotation() const;
	const Vector3& GetScale() const;
	const Matrix& GetWorldToLocalMatrix();
	const Matrix& GetLocalToWorldMatrix();
private:
	void UpdateMatrix();
protected:
	Scene *m_scene;
	Vector3 m_pos;
	Quaternion m_rot;
	Vector3 m_scale;
	Matrix m_local2world_mat;
	Matrix m_world2local_mat;
	bool m_mat_dirty;
};

typedef std::vector<SceneNode*> SceneNodeVec;
