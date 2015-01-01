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
#include "lightnode.h"
#include "cameranode.h"
#include "postprocess.h"
#include "postprocessdof.h"
#include "scene.h"
#include "rendernode.h"

RenderNode::RenderNode()
{

}

void RenderNode::ClearLight()
{
	m_lights.clear();
}

void RenderNode::AddLight(LightNode *light)
{
	LightVec::iterator itr = m_lights.begin();
	for (; itr != m_lights.end(); ++itr)
	{
		if (*itr == light) {
			break;
		}
	}
	if (itr == m_lights.end()) {
		m_lights.push_back(light);
	}
}

void RenderNode::DelLight(LightNode *light)
{
	LightVec::iterator itr = m_lights.begin();
	for (; itr != m_lights.end(); ++itr)
	{
		if (*itr == light) {
			m_lights.erase(itr);
			break;
		}
	}
}