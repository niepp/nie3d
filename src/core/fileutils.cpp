#include "core.h"
#include "fileutils.h"

#if PLATFORM == PLATFORM_WIN32
#ifdef _DEBUG
#pragma comment(lib, "../../../thirdparty/ZLib/win/debug/zlib.lib")
#else
#pragma comment(lib, "../../../thirdparty/ZLib/win/release/zlib.lib")
#endif
#endif

FileUtils* FileUtils::Instance()
{
	static FileUtils sFileUtils;
	return &sFileUtils;
}

