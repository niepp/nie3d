#include <algorithm>
#include "core/core.h"

#include "ogl.h"
#include "ogldefine.h"
#include "ogltexture.h"

OGLTexture::OGLTexture() : m_gl_tex(0)
, m_Width(0)
, m_Height(0)
, m_Depth(0)
, mAddressState(cAddressWarp)
, mFilterState(cFilterTrilinear)
, m_usage(cUsageTexture)
, m_MipLevels(0)
, m_Compressed(false)
{
}

OGLTexture::~OGLTexture()
{
	this->Destroy();
}

void OGLTexture::SetAddressMode(const AddressState& texaddress)
{
	mAddressState = texaddress;
	GLBINDTEXTURE(m_TextureType, m_gl_tex);

	switch(texaddress)
	{
	case cAddressWarp:
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case cAddressClamp:
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case cAddressBorder:
#if PLATFORM != PLATFORM_IOS && PLATFORM != PLATFORM_ANDROID
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
#endif
		break;
	case cAddressMirror:
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	default:
		assert(0);
		break;
	}
}

void OGLTexture::SetFilterMode(const FilterState& texfilter)
{
	mFilterState = texfilter;
	GLBINDTEXTURE(m_TextureType, m_gl_tex); //m_TextureType = GL_TEXTURE_2D / GL_TEXTURE_3D
	switch(texfilter)
	{
	case cFilterTrilinear:
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#ifdef GL_TEXTURE_MAX_LEVEL
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MAX_LEVEL, ((m_MipLevels == 0) ? 0 : m_MipLevels - 1));
		GLTEXPARAMETERF(m_TextureType, GL_TEXTURE_LOD_BIAS, 0.0f);
#endif
		break;
	case cFilterBilinear:
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case cFilterPoint:
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
#ifdef GL_TEXTURE_MAX_LEVEL
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MAX_LEVEL, ((m_MipLevels == 0) ? 0 : m_MipLevels - 1));
		GLTEXPARAMETERF(m_TextureType, GL_TEXTURE_LOD_BIAS, 0.0f);
#endif
		break;
	case cFilterAni:
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
#ifdef GL_TEXTURE_MAX_LEVEL
		GLTEXPARAMETERI(m_TextureType, GL_TEXTURE_MAX_LEVEL, ((m_MipLevels == 0)?0:m_MipLevels-1));
		GLTEXPARAMETERF(m_TextureType, GL_TEXTURE_LOD_BIAS, -1.5f);
#endif
		break;//do nothing now...
	default:
		assert(0);
		break;
	}

}

Bool OGLTexture::Create(const TextureDesc &desc, const TextureData *ptexdata)
{
	m_PixelFormat = desc.format;

	assert(desc.depth == 32 || desc.depth == 24);

	TestGLError();

	GLGENTEXTURES(1, &m_gl_tex);

	GLenum texturetype;
	switch (desc.type)
	{
	case cTexture2D:
		m_Width = desc.width;
		m_Height = desc.height;
		texturetype = GL_TEXTURE_2D;
		break;
	default:
		assert(0);
		break;
	}

	m_TextureType = texturetype;
	m_glformat = g_FallbackPath[m_PixelFormat][0];
	GLBINDTEXTURE(texturetype, m_gl_tex);

	if(desc.miplevels != 0)
	{
		m_MipLevels = desc.miplevels;
	}
	else
	{
		int mips = 1;
		Uint32 w = m_Width;
		Uint32 h;
		if(desc.type == cTexture2D)
		{
			h = m_Height;
		}
		else
		{
			h = w;
		}
		while ((w != 1) || (h != 1))
		{
			++mips;
			w = std::max<Uint32>(1, w / 2);
			h = std::max<Uint32>(1, h / 2);
		}
		m_MipLevels = mips;
	}

	if(desc.type == cTexture2D)
	{
		TestGLError();

		for(Uint32 i = 0; i < m_MipLevels; ++i)
		{
			if(m_Compressed == false)
			{
				if (desc.usage == cUsageTexture)
				{
					GLenum format = (desc.depth == 32) ? GL_RGBA : GL_RGB;
					GLTEXIMAGE2D(GL_TEXTURE_2D, i, GL_RGBA, Max(m_Width>>i, 1U), Max(m_Height>>i, 1U), 0, format, GL_UNSIGNED_BYTE, ptexdata == NULL ? NULL : ptexdata->bits);

					LogUtils::Instance()->LogInfo("file = %s, w = %d, h = %d", desc.filepath, Max(m_Width>>i, 1U), Max(m_Height>>i, 1U));

				}
				else if (desc.usage == cUsageDepthStencil)
				{
					// gles 2.0 does not support GL_DEPTH_COMPONENT etc, it need OES_depth_texture
					GLTEXIMAGE2D(GL_TEXTURE_2D, i, GL_DEPTH_COMPONENT, Max(m_Width>>i, 1U), Max(m_Height>>i, 1U), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, ptexdata == NULL ? NULL : ptexdata->bits);		
				}
				//else if (desc.usage == cUsageFloat)
				//{
				//	GLTEXIMAGE2D(GL_TEXTURE_2D, i, GL_RGBA16F, Max(m_Width>>i, 1U), Max(m_Height>>i, 1U), 0, GL_RGBA, GL_HALF_FLOAT, ptexdata == NULL ? NULL : ptexdata->bits);
				//}
				TestGLError();
			}
			else
			{
                // todo
				Uint32 slicepitch;
				Uint32 width, height;
				width = std::max<Uint32>(m_Width>>i,1U);
				height = std::max<Uint32>(m_Height>>i,1U);
                slicepitch = ((width + 3)/4) * ((height + 3)/4) * 16;
				GLCOMPRESSEDTEXIMAGE2D(GL_TEXTURE_2D, i, m_glformat, width, height, 0, slicepitch, ptexdata == NULL ? NULL : ptexdata->bits);
				TestGLError();
			}
		}
		GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		TestGLError();

		GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GLTEXPARAMETERI(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		TestGLError();

	}
	else
	{
		assert(0);
	}

	TestGLError();

	return true;

}

void OGLTexture::GetDesc(TextureDesc &desc)
{
	desc.width = m_Width;
	desc.height = m_Height;
	desc.depth = m_Depth;

	if (m_TextureType == GL_TEXTURE_2D)
	{
		desc.type = cTexture2D;
	}
	else
	{
		assert(0);
	}

	desc.usage = m_usage;
	desc.miplevels = m_MipLevels;
	desc.format = m_PixelFormat;

}

void OGLTexture::GetSize(unsigned int& nWidth, unsigned int& nHeight)
{
	nWidth = m_Width;
	nHeight = m_Height;
}

void OGLTexture::Destroy()
{
	GLDELETETEXTURES(1, &m_gl_tex);
}

