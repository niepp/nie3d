#include "core/core.h"

#include "engine.h"
#include "resource.h"
#include "texture.h"

#if PLATFORM == PLATFORM_WIN32
#ifdef _DEBUG
#pragma comment(lib, "../../../thirdparty/ZLib/win/debug/zlib.lib")
#pragma comment(lib, "../../../thirdparty/libpng/win/debug/libpng.lib")
#else
#pragma comment(lib, "../../../thirdparty/ZLib/win/release/zlib.lib")
#pragma comment(lib, "../../../thirdparty/libpng/win/release/libpng.lib")
#endif
#endif

typedef struct {
	Uint8* data;
	Uint32 size;
	Uint32 offset;
}ImageSource;

static void pngReaderCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	ImageSource* isource = (ImageSource*)png_get_io_ptr(png_ptr);
	if(isource->offset + length <= isource->size) {
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	}
	else {
		png_error(png_ptr,"pngReaderCallback failed");
	}
}

Texture::Texture(const Filepath &file_path, Uint32 miplevels) : Resource(file_path, cResTypeTexture)
{
	m_desc.width = 0;
	m_desc.height = 0;
	m_desc.format = cPixelFormatR8G8B8A8;
	m_desc.filepath[0] = 0;
	m_desc.depth = 32;
	m_desc.type = cTexture2D;
	m_desc.miplevels = miplevels;
	m_desc.usage = cUsageTexture;
}

bool Texture::Load()
{
	Uint32 nDatalen = 0;
	Uint8 *pData = FileUtils::Instance()->ReadBinaryFile(m_file_path.AsCharPtr(), &nDatalen);

	png_structp png_ptr;
    png_infop info_ptr;
    int width, height, rowBytes;
    png_byte color_type;  //可以是PNG_COLOR_TYPE_RGB,PNG_COLOR_TYPE_PALETTE.......等
    png_byte bit_depth;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,NULL,NULL);
    if (!png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }

    ImageSource imgsource;
    imgsource.data = pData;
    imgsource.size = nDatalen;
    imgsource.offset = 0;
    //define our own callback function for I/O instead of reading from a file
    png_set_read_fn(png_ptr, &imgsource, pngReaderCallback );


    /* **************************************************
     * The low-level read interface in libpng (http://www.libpng.org/pub/png/libpng-1.2.5-manual.html)
     * **************************************************
     */
    png_read_info(png_ptr, info_ptr);
	if (!png_ptr || !info_ptr)
	{
		return false;
	}
	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	rowBytes = png_get_rowbytes(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

    // Convert stuff to appropriate formats!
    if(color_type==PNG_COLOR_TYPE_PALETTE) {
        png_set_packing(png_ptr);
        png_set_palette_to_rgb(png_ptr); //Expand data to 24-bit RGB or 32-bit RGBA if alpha available.
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);

    //Expand paletted or RGB images with transparency to full alpha channels so the data will be available as RGBA quartets.
    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png_ptr);
    }

	Uint32 img_size = width * height * 4;
    Uint8* rgba = new Uint8[img_size];  //each pixel(RGBA) has 4 bytes
    png_bytep *row_pointers = new png_bytep[height];
	for (unsigned short i = 0; i < height; ++i) {
		row_pointers[i] = rgba + i * width * 4;
	}
    png_read_image(png_ptr, row_pointers);

    //unlike store the pixel data from top-left corner, store them from bottom-left corner for OGLES Texture drawing...
    int pos = (width * height * 4) - (4 * width);
    for(int row = 0; row < height; row++)
    {
       for(int col = 0; col < (4 * width); col += 4)
       {
           rgba[pos++] = row_pointers[row][col];        // red
           rgba[pos++] = row_pointers[row][col + 1];    // green
           rgba[pos++] = row_pointers[row][col + 2];    // blue
           rgba[pos++] = row_pointers[row][col + 3];    // alpha
       }
       pos = (pos - (width * 4)*2); //move the pointer back two rows
    }

    //clean up after the read, and free any memory allocated
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	m_desc.width = width;
	m_desc.height = height;
	m_desc.depth = 32;
	m_desc.format = cPixelFormatR8G8B8A8;
	m_desc.usage = cUsageTexture;
	strcpy(m_desc.filepath, m_file_path.AsCharPtr());

	m_texdata.bits = rgba;
	m_texdata.img_size = img_size;
	m_texdata.format = m_desc.format;

	m_tex = GetRenderer()->CreateTexture(m_desc, &m_texdata);

	return true;

}

#if PLATFORM == PLATFORM_WIN32
Bool SaveTextureBMP(const TextureDesc &tex_desc, const Uint8 *tex_data, const Filepath &file_path)
{
	unsigned char* buffer = NULL;
	int  blength = 0;
	blength = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (tex_desc.width * 4) * tex_desc.height;
	buffer = new unsigned char[blength];
	memset(buffer, 0, blength);

	BITMAPINFOHEADER bi;
	bi.biSize           =  sizeof(BITMAPINFOHEADER);  
	bi.biWidth          =  tex_desc.width;
	bi.biHeight         =  tex_desc.height;  
	bi.biPlanes         =  1;  
	bi.biBitCount       =  32;  
	bi.biCompression    =  BI_RGB;  
	bi.biSizeImage      =  0;  
	bi.biXPelsPerMeter  =  0;  
	bi.biYPelsPerMeter  =  0;  
	bi.biClrImportant   =  0;  
	bi.biClrUsed        =  0; 

	BITMAPFILEHEADER bmfHdr;
	bmfHdr.bfType = 0x4D42;  // "BM"
	bmfHdr.bfSize = blength;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 

	memcpy( buffer, &bmfHdr, sizeof(BITMAPFILEHEADER));
	memcpy( buffer + sizeof(BITMAPFILEHEADER), &bi, sizeof(BITMAPINFOHEADER));
	unsigned char *dst = (unsigned char*)(buffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));

	for (Uint32 i = 0; i < tex_desc.height; ++i)
	{
		for (Uint32 j = 0; j < tex_desc.width; ++j)
		{
			Uint32 index = i * tex_desc.width * 4 + j * 4;
			dst[index + 0] = tex_data[index + 0];
			dst[index + 1] = tex_data[index + 1];
			dst[index + 2] = tex_data[index + 2];
			dst[index + 3] = tex_data[index + 3];	
		}
	}

	Bool hr = FileUtils::Instance()->WriteBinaryFile(file_path.AsCharPtr(), buffer, blength);

	return hr;

}
#endif

