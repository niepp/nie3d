#pragma once

class OGLTexture : public ITexture
{
public:
	OGLTexture();
	~OGLTexture();

	void Destroy();

	virtual void SetAddressMode(const AddressState& texaddress);
	virtual void SetFilterMode(const FilterState& texfilter);
	virtual const AddressState& GetAddressMode() const { return mAddressState; }
	virtual const FilterState& GetFilterMode() const { return mFilterState; }

	virtual void GetDesc(TextureDesc &desc);
	virtual void GetSize(unsigned int& nWidth, unsigned int& nHeight);

	Bool Create(const TextureDesc &desc, const TextureData *ptexdata);
	const GLuint& GetGLTex() { return m_gl_tex; }

private:
	PixelFormat		                m_PixelFormat;    
	Bool							m_Compressed;
	Uint32							m_Width;
	Uint32							m_Height;
	Uint32							m_Depth;
	TextureUsage					m_usage;
	Uint32							m_MipLevels;

	GLuint					        m_gl_tex;
	GLint							m_glformat;
	GLenum							m_TextureType;

	AddressState mAddressState;
	FilterState mFilterState;

};
