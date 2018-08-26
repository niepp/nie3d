#pragma once

class OGLRenderer : public IRenderer
{
public:
	OGLRenderer();
	~OGLRenderer();
	Bool Create(const RendererDesc &desc);
	void Destroy();
	Bool InitDevice(const RendererDesc &desc);
//#if PLATFORM == PLATFORM_ANDROID
//	void Resume(const RendererDesc &desc);
//#endif

public:
	virtual ITexture*		CreateTexture(const TextureDesc &texdesc, const TextureData *ptexdata=NULL );
	virtual IIndexBuffer*	CreateIndexBuffer(UsageType usage, Uint32 num_index, IndexType index_type);
	virtual IVertexBuffer*	CreateVertexBuffer(UsageType usage, Uint32 buflen, Uint32 stride);
	virtual IInputLayout*	CreateInputLayout(const InputElement *pelements, Uint32 nelement);
	virtual IMaterialTemplate* CreateMaterialTemplate(const char *path);

	virtual void SetVertexBuffer(IVertexBuffer *pbuf, Uint32 offsetbytes = 0, Uint32 stride = 0);
	virtual void SetInputLayout(IInputLayout *playout);
	virtual void Draw(PrimitiveType topo, Uint32 nvert, IIndexBuffer *pindexbuf = NULL, Uint32 startindex = 0);

	virtual IRenderTarget*	CreateRenderTarget(Uint32 width, Uint32 height);
	virtual IRenderTarget*	GetMainTarget();
	virtual IRenderTarget*	CreateAdditionalWindowTarget(const RendererDesc &desc);

	virtual Bool SetRenderState(RenderState rs);
	virtual void RestoreDefaultState(Bool samplerState = true);

public:
	OGLWinRenderTarget* FindWindowTarget(HWND hwnd);

	void AddMaterialTemplate(const String &name, MaterialTemplate *pcoremtl);
	MaterialTemplate* FindMaterialTemplate(const String &name);

private:
	char m_shaderDir[256];
	IInputLayout *m_lastLayout;
	OGLDevice m_device;
	RenderState m_curRenderState;

	std::vector<OGLWinRenderTarget*>m_WindowTargets;

	typedef std::map<String, MaterialTemplate*> MaterialTemplateMap;
	MaterialTemplateMap m_mtlTempMap;

};

extern "C" {
	OGLRENDERER_API IRenderer* CreateRenderer(const RendererDesc &desc);
};

