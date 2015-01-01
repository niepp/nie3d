#include "../core.h"
#include "renderstate.h"

static const char* s_pRenderStateName[] =
{
	"cColorWriteMask",

	"cBlendEnable",
	"cBlendFunc",
	"cSrcBlend",
	"cDestBlend",

	"cDepthTestEnable",
	"cDepthWriteEnable",
	"cDepthFunc",

	"cStencilEnable",

	"cAlphaTestEnable",
	"cAlphaFunc",
	"cAlphaRefValue",

	"cCullMode",
	"cFillMode",

	"cClipPlane",
	""
};

const char* RenderState::TypeToName(RenderStateType type)
{
	return s_pRenderStateName[type];
}

RenderStateType RenderState::NameToType(const char *pname)
{
	for (Uint32 i = 0; i < cRenderStateTypeCount; ++i)
	{
		if (strcmp(s_pRenderStateName[i], pname) == 0) {
			return (RenderStateType)i;
		}
	}
	assert(0);
	return cColorWriteMask;
}

Bool InitBlendState()
{
	RenderState::BlendState[cBlendOpaque].SetRenderState(cBlendEnable, false);

	RenderState::BlendState[cBlendAlphaBlend].SetRenderState(cBlendEnable, true);
	RenderState::BlendState[cBlendAlphaBlend].SetRenderState(cSrcBlend, cBlendSrcAlpha);
	RenderState::BlendState[cBlendAlphaBlend].SetRenderState(cDestBlend, cBlendInvSrcAlpha);
	RenderState::BlendState[cBlendAlphaBlend].SetRenderState(cDepthWriteEnable, false);

	RenderState::BlendState[cBlendAlphaTest].SetRenderState(cAlphaTestEnable, true);
	RenderState::BlendState[cBlendAlphaTest].SetRenderState(cAlphaFunc, cCmpGreater);
	RenderState::BlendState[cBlendAlphaTest].SetRenderState(cAlphaRefValue, 0x78);

	RenderState::BlendState[cBlendAddMulColor].SetRenderState(cBlendEnable, true);
	RenderState::BlendState[cBlendAddMulColor].SetRenderState(cSrcBlend, cBlendSrcColor);
	RenderState::BlendState[cBlendAddMulColor].SetRenderState(cDestBlend, cBlendDestColor);
	RenderState::BlendState[cBlendAddMulColor].SetRenderState(cDepthWriteEnable, false);

	RenderState::BlendState[cBlendAddMulAlpha].SetRenderState(cBlendEnable, true);
	RenderState::BlendState[cBlendAddMulAlpha].SetRenderState(cSrcBlend, cBlendSrcAlpha);
	RenderState::BlendState[cBlendAddMulAlpha].SetRenderState(cDestBlend, cBlendDestAlpha);
	RenderState::BlendState[cBlendAddMulAlpha].SetRenderState(cDepthWriteEnable, false);

	return true;

}

RenderState RenderState::BlendState[cBlendModeCount];
Bool inited = InitBlendState();


