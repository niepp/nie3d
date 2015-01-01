#include "core/core.h"

#include "oglrenderer/ogl.h"
#include "oglrenderer/ogldefine.h"
#include "oglrenderer/ogltexture.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"
#include "material.h"
#include "meshresource.h"
#include "material.h"
#include "rendercontext.h"
#include "utilityrenderer.h"
#include "scenenode.h"
#include "cameranode.h"
#include "lightnode.h"
#include "rendernode.h"

#include "postprocess.h"

QuadRender::QuadRender()
{
	IRenderer *renderer = GetRenderer();
	InputElement elements0[] = {
		{0, 0, cInputUsagePosition, 0, cPixelFormatR32G32B32_Float},
	};

	m_quad_layout = renderer->CreateInputLayout(elements0, ArraySize(elements0));

	struct _QuadVertex {
		Vector3 pos;
	};

	_QuadVertex vb[4];
	vb[0].pos.Set(-1.0f, -1.0f, 0);
	vb[1].pos.Set(1.0f, -1.0f, 0);
	vb[2].pos.Set(-1.0f, 1.0f, 0);
	vb[3].pos.Set(1.0f, 1.0f, 0);

	m_quad_vb = renderer->CreateVertexBuffer(cUsageStatic, 4 * sizeof(_QuadVertex), sizeof(_QuadVertex));
	m_quad_vb->Update(vb, sizeof(vb));

}

QuadRender::~QuadRender()
{

}

void QuadRender::DrawQuad(ITexture *src, IShader *shader)
{
	IRenderer *renderer = GetRenderer();
	assert(shader);

	shader->BeginPass(0);
	shader->SetTexture("_MainTex", src);

	RenderState rs;
	rs.SetRenderState(cDepthTestEnable, 0);
//	rs.SetRenderState(cDepthWriteEnable, 0);

	renderer->SetRenderState(rs);
	renderer->SetVertexBuffer(m_quad_vb);
	renderer->SetInputLayout(m_quad_layout);
	renderer->Draw(cTriangleStrip, 4, 0);
	shader->EndPass();
}

