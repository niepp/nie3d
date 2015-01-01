#pragma once

#include "LibPNG/png.h"

class ENGINE_API Texture : public Resource
{
public:
	Texture(const Filepath &file_path, Uint32 miplevels = 0);
	virtual bool Load();
	ITexture* GetTexture() { return m_tex; }
private:
public:
	TextureDesc m_desc;
	TextureData m_texdata;
	ITexture *m_tex;

};

#if PLATFORM == PLATFORM_WIN32
ENGINE_API Bool SaveTextureBMP(const TextureDesc &tex_desc, const Uint8 *tex_data, const Filepath &file_path);
#endif
