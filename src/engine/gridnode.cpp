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
#include "gridnode.h"

GridNode::GridNode()
{

}

void GridNode::Update()
{

}

void GridNode::FillRenderContext(RenderContextQueue *rc_queue, const RenderEnv *env)
{

}

void GridNode::RenderDebug(UtilityRenderer *renderer)
{
	renderer->DrawLine(Vector3(0,0,0), Vector3(1,0,0), Color::Red);
	renderer->DrawLine(Vector3(0,0,0), Vector3(0,1,0), Color::Green);
	renderer->DrawLine(Vector3(0,0,0), Vector3(0,0,1), Color::Blue);
}
