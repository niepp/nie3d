#pragma once
#ifndef __IRENDERER_H__
#define __IRENDERER_H__

#include <vector>
#include <map>

struct ShaderMacro;

struct TextureDesc
{
	char filepath[cMaxPath];
	TextureType type;
	PixelFormat format;
	TextureUsage usage;
	Uint32 miplevels;
	Uint32 width;
	Uint32 height;
	Uint32 depth;
};

struct TextureData
{
	void *bits;
	Uint32 row_pitch;
	Uint32 img_size;
	PixelFormat format;
};

struct TechniqueDesc
{
	String tech_name;
	String vs_entry;
	String ps_entry;
};

struct InputElement
{
	Uint32 slot;
	Uint32 offset;
	InputUsage usage;
	Uint32 index;
	PixelFormat format;
};

struct RendererDesc
{
	Int32 adapter;
	Bool bfullscreen;
	Bool debug_vs;
	Bool debug_ps;
	Bool interval;
	Uint32 width;
	Uint32 height;
	Uint32 colorbits;
	Uint32 alphabits;
	Uint32 depthbits;
	Uint32 stencilbits;
	Uint32 msaa_level;	//  0, 1,2x 2,4x
	void* context_obj;
};

class IIndexBuffer
{
public:
	virtual void* Lock(Uint32 flags) = 0;
	virtual void Unlock() = 0;
	virtual bool IsLocked() = 0;
	virtual void Update(const void *data, Uint32 size = 0) = 0;
	virtual void SetVertexRange(Uint32 start, Uint32 n_vert) = 0;
};

class IVertexBuffer
{
public:
	virtual void* Lock(Uint32 flags) = 0;
	virtual void Unlock() = 0;
	virtual bool IsLocked() = 0;
	virtual void Update(const void *data, Uint32 size = 0) = 0;
	virtual Uint32 GetNumOfVertexes() = 0;
};

class IVertexArrayObject
{
protected:
	virtual ~IVertexArrayObject(){ }
};

class IInputLayout
{
protected:
	virtual ~IInputLayout(){ }
};

class ITexture
{
public:
	struct LockDesc
	{
		Uint32 flags;
		Uint32 iface;
		Uint32 level;
	};

	struct LockResult
	{
		void *bits;
		Uint32 row_pitch;
		Uint32 slice_pitch;
	};

	virtual void SetAddressMode(const AddressState &texaddress) = 0;
	virtual void SetFilterMode(const FilterState &texfilter) = 0;
	virtual const AddressState& GetAddressMode() const = 0;
	virtual const FilterState& GetFilterMode() const = 0;

	virtual void GetDesc(TextureDesc &desc) = 0;
	virtual void GetSize(Uint32 &width, Uint32 &height) = 0;

};

class IShader
{
public:
	virtual Uint32 Begin() = 0;
	virtual void End() = 0;
	virtual void BeginPass(Uint32 ipass = 0) = 0;
	virtual void EndPass() = 0;

	virtual bool SetConstant(const String &name, const void *pdata) = 0;
	virtual bool SetTexture( const String &name, ITexture *ptexture ) = 0;
};

class IRenderTarget
{
public:
	struct LockResult
	{
		int pitch;
		void *bits;
		LockResult() : pitch(0), bits(NULL) { }
	};

	virtual bool BeginScene(Uint32 index = 0) = 0;
	virtual void EndScene(Uint32 index = 0) = 0;
	virtual void Present() = 0;
	virtual void GetSize(Uint32 &width, Uint32 &height) = 0;
	virtual bool Lock(LockResult &result, const RectInt *rect = NULL) = 0;
	virtual void Unlock() = 0;
	virtual void AttachTexture(ITexture *depth_tex, Uint32 depth_level,	ITexture *rt_tex, Uint32 rt_level) = 0;
	virtual void SetClearParam(Uint32 flags, const Color &color, Float z, Uint32 stencil) = 0;
	virtual void HandlePossibleSizeChange() = 0;

};

class IMaterialTemplate : public RefObject
{
public:
	class ShaderParamDesc
	{
	public:
		String m_name;
		String m_ui_name;
		Variant m_def_value;
		Variant m_min_value;
		Variant m_max_value;
		String m_ui_type;
		bool m_display;
	};

	class ShaderMacroDesc
	{
	public:
		String m_name;
		String m_ui_name;
		bool m_macro; // default value
		bool m_display;
	};

public:
	virtual Uint32 GetParamCount() = 0;
	virtual Uint32 FindParamByName(const String &name) = 0;

	virtual Uint32 GetMacroCount() = 0;
	virtual Uint32 FindMacroByName(const String &name) = 0;

	virtual const ShaderParamDesc* GetParamDesc(Uint32 index) const = 0;
	virtual const ShaderMacroDesc* GetMacroDesc(Uint32 index) const = 0;

	virtual IShader* GetShader(const ShaderMacro& macro, const String& techName) = 0;

};

class IRenderer
{
public:
	virtual ITexture*		CreateTexture(const TextureDesc &texdesc, const TextureData *ptexdata = NULL) = 0;
	virtual IIndexBuffer*	CreateIndexBuffer(UsageType usage, Uint32 num_index, IndexType index_type) = 0;
	virtual IVertexBuffer*	CreateVertexBuffer(UsageType usage, Uint32 buflen, Uint32 stride) = 0;
	virtual IInputLayout*	CreateInputLayout(const InputElement *pelements, Uint32 nelement) = 0;
	virtual IMaterialTemplate* CreateMaterialTemplate(const char *path) = 0;

	virtual IRenderTarget*	CreateRenderTarget(Uint32 width, Uint32 height) = 0;
	virtual IRenderTarget*	GetMainTarget() = 0;
	virtual IRenderTarget*	CreateAdditionalWindowTarget(const RendererDesc &desc) = 0;

	virtual Bool SetRenderState(RenderState rs) = 0;
	virtual void RestoreDefaultState(Bool samplerState = true) = 0;

	virtual void SetVertexBuffer(IVertexBuffer *pbuf, Uint32 offsetbytes = 0, Uint32 stride = 0) = 0;
	virtual void SetInputLayout(IInputLayout *playout) = 0;
	virtual void Draw(PrimitiveType topo, Uint32 nvert, IIndexBuffer *pindexbuf = NULL, Uint32 startindex = 0) = 0;

};

#endif //__IRENDERER_H__