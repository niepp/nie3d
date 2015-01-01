#pragma once
#include <vector>
#include <map>

class ENGINE_API RenderNode : public SceneNode {
public:
	RenderNode();
	void ClearLight();
	void AddLight(LightNode *light);
	void DelLight(LightNode *light);
protected:
	LightVec m_lights;
};
