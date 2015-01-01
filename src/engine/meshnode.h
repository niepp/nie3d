#pragma once
#include <vector>

class ENGINE_API MeshNode : public RenderNode {
public:
	MeshNode(MeshResource *mesh_res);
	virtual void Update();
	virtual void FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env);
	virtual void RenderDebug(UtilityRenderer *renderer);
private:
	MeshResource *m_mesh_res;
	Material *m_mtl;
};
