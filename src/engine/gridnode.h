#pragma once

class ENGINE_API GridNode : public SceneNode {
public:
	GridNode();
	virtual void Update();
	virtual void FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env);
	virtual void RenderDebug(UtilityRenderer *renderer);
private:
	
};
