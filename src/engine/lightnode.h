#pragma once
#include <vector>

enum LightType{
	cLightDirection,
	cLightPoint,
	cLightSpot,
	cLightTypeCount,
};

class ENGINE_API LightNode : public SceneNode {
public:
	LightNode(LightType lightType);
	virtual void Update();
	virtual void RenderDebug(UtilityRenderer *renderer);
	void SetLightColor(const Color& color) { m_light_color = color; }
	const Color& GetLightColor() const { return m_light_color; }
	const LightType& GetLightType() const { return m_light_type; }
private:
	LightType m_light_type;
	Color m_light_color;
};

class ENGINE_API PointLight : public LightNode {
public:
	PointLight();
	PointLight(Float range, Float attenuation, Float intensity);
	virtual void Update();
	virtual void RenderDebug(UtilityRenderer *renderer);
	void SetRange(Float range) { m_range = range; }
	void SetAttenuation(Float atten) { m_attenuation = atten; }
	void SetIntensity(Float inten) { m_intensity = inten; }
	Float GetRange() const { return m_range; }
	Float GetAttenuation() const { return m_attenuation; }
	Float GetIntensity() const { return m_intensity; }
private:
	Float m_range;
	Float m_attenuation;
	Float m_intensity;
};

typedef std::vector<LightNode*> LightVec;