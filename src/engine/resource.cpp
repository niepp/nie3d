#include "core/core.h"
#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogldevice.h"
#include "oglrenderer/oglgeobuffer.h"
#include "oglrenderer/oglinputlayout.h"
#include "oglrenderer/ogltexture.h"
#include "oglrenderer/oglmaterialtemplate.h"
#include "oglrenderer/oglwindowtarget.h"
#include "oglrenderer/oglrenderer.h"

#include "engine.h"
#include "resource.h"

// todo: remove g_renderer
ENGINE_API IRenderer *g_renderer(NULL);
IRenderer* GetRenderer()
{
	return g_renderer;
}
