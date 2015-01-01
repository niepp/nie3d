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
#include "lightnode.h"

LightNode::LightNode(LightType lightType) : m_light_type(lightType)
{
}

void LightNode::Update()
{

}

void LightNode::RenderDebug(UtilityRenderer *renderer)
{

}

PointLight::PointLight() : LightNode(cLightPoint)
{
}

PointLight::PointLight(Float range, Float attenuation, Float intensity) 
	: m_range(range), m_attenuation(attenuation), m_intensity(intensity)
	, LightNode(cLightPoint)
{

}

void PointLight::Update()
{

}

void PointLight::RenderDebug(UtilityRenderer *renderer)
{
	const Vector3 &p = GetPosition();
	renderer->DrawLine(p, p + Vector3(0.45f, 0, 0), GetLightColor(), false);
	renderer->DrawLine(p, p + Vector3(0, 0.45f, 0), GetLightColor(), false);
	renderer->DrawLine(p, p + Vector3(0, 0, 0.45f), GetLightColor(), false);
}
