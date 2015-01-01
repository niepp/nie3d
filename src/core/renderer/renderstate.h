#pragma once
#ifndef __RENDER_STATE_H__
#define __RENDER_STATE_H__

struct CORE_API RenderState 
{
	union
	{
		struct
		{
			Uint32 reserved:18;
			Uint32 clip_plane:6;

			Uint32 cull_mode:2;
			Uint32 fill_mode:2;
			Uint32 blend_op:4;

			Uint32 alpha_ref:8;
			Uint32 alpha_func:3;
			Uint32 alpha_test_enable:1;
			Uint32 depth_bias:1;

			Uint32 stencil_enable:1;
			Uint32 depth_func:3;
			Uint32 depth_write_enable:1;
			Uint32 depth_test_enable:1;

			Uint32 src_blend:4;
			Uint32 dest_blend:4;
			Uint32 alpha_blend:1;

			Uint32 color_write_r:1;
			Uint32 color_write_g:1;
			Uint32 color_write_b:1;
			Uint32 color_write_a:1;
		};

		Uint64 cmp_data;
	};

	static RenderState BlendState[cBlendModeCount];

	RenderState();
	void SetDefault();
	void SetRenderState(RenderStateType rs_type, Uint32 state_value);
	void GetRenderState(RenderStateType rs_type, Uint32 state_value) const;

	bool operator ==(const RenderState &rs) const;
	bool operator !=(const RenderState &rs) const;
	bool operator<(const RenderState &rs) const;
	RenderState& operator =(const RenderState &rs);

	static const char* TypeToName(RenderStateType type);
	static RenderStateType NameToType(const char *pname);

};

inline bool RenderState::operator ==(const RenderState &rs) const
{
	return (cmp_data == rs.cmp_data);
}

inline bool RenderState::operator !=(const RenderState &rs) const
{
	return (cmp_data != rs.cmp_data);
}

inline bool RenderState::operator<(const RenderState &rs) const
{
	return cmp_data < rs.cmp_data;
}

inline RenderState& RenderState::operator =(const RenderState &rs)
{
	cmp_data = rs.cmp_data;
	return *this;
}

inline RenderState::RenderState()
{
	SetDefault();
}

inline void RenderState::SetDefault()
{
	cmp_data = 0;

	SetRenderState(cColorWriteMask, 15);

	SetRenderState(cBlendEnable, 0);
	SetRenderState(cSrcBlend, cBlendSrcAlpha);
	SetRenderState(cDestBlend, cBlendInvSrcAlpha);
	SetRenderState(cBlendFunc, cBlendFuncAdd);

	SetRenderState(cDepthTestEnable, 1);
	SetRenderState(cDepthFunc, cCmpLEqual);
	SetRenderState(cDepthWriteEnable, 1);

	SetRenderState(cStencilEnable, 0);

	SetRenderState(cAlphaTestEnable, 0);
	SetRenderState(cAlphaFunc, cCmpGreater);
	SetRenderState(cAlphaRefValue, 0x78);

	SetRenderState(cCullMode, cCullCCW);
	SetRenderState(cFillMode, cFillSolid);

	SetRenderState(cClipPlane, 0);

}

inline void RenderState::SetRenderState(RenderStateType rs_type, Uint32 state_value)
{
	switch(rs_type)
	{
	case cColorWriteMask:
		color_write_r = (state_value & 0x00000001);
		color_write_g = (state_value & 0x00000002) >> 1;
		color_write_b = (state_value & 0x00000004) >> 2;
		color_write_a = (state_value & 0x00000008) >> 3;
		break;
	case cBlendEnable:
		alpha_blend = state_value;
		break;
	case cBlendFunc:
		blend_op = state_value;
		break;
	case cSrcBlend:
		src_blend = state_value;
		break;
	case cDestBlend:
		dest_blend = state_value;
		break;
	case cDepthTestEnable:
		depth_test_enable = state_value;
		break;
	case cDepthWriteEnable:
		depth_write_enable = state_value;
		break;
	case cDepthFunc:
		depth_func = state_value;
		break;
	case cStencilEnable:
		stencil_enable = state_value;
		break;
	case cAlphaTestEnable:
		alpha_test_enable = state_value;
		break;
	case cAlphaFunc:
		alpha_func = state_value;
		break;
	case cAlphaRefValue:
		alpha_ref = state_value;
		break;
	case cCullMode:
		cull_mode = state_value;
		break;
	case cFillMode:
		fill_mode = state_value;
		break;
	case cClipPlane:
		clip_plane = 0;
		break;
	default:
		break;
	}
}

inline void RenderState::GetRenderState(RenderStateType rs_type, Uint32 state_value) const
{
	switch(rs_type)
	{
	case cColorWriteMask:
		state_value = (color_write_a << 3) | (color_write_a << 2) | (color_write_a << 1) | color_write_r;
		break;
	case cBlendEnable:
		state_value = alpha_blend ;
		break;
	case cBlendFunc:
		state_value = blend_op;
		break;
	case cSrcBlend:
		state_value = src_blend;
		break;
	case cDestBlend:
		state_value = dest_blend;
		break;
	case cDepthTestEnable:
		state_value = depth_test_enable;
		break;
	case cDepthWriteEnable:
		state_value = depth_write_enable;
		break;
	case cDepthFunc:
		state_value = depth_func;
		break;
	case cStencilEnable:
		state_value = stencil_enable;
		break;
	case cAlphaTestEnable:
		state_value = alpha_test_enable;
		break;
	case cAlphaFunc:
		state_value = alpha_func;
		break;
	case cAlphaRefValue:
		state_value = alpha_ref;
		break;
	case cCullMode:
		state_value = cull_mode;
		break;
	case cFillMode:
		state_value = fill_mode;
		break;
	case cClipPlane:
		state_value = clip_plane;
		break;
	default:
		break;
	}
}

#endif // __RENDER_STATE_H__