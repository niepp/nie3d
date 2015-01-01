#pragma once

#ifndef __ENGINE_H__
#define __ENGINE_H__

#if (PLATFORM == PLATFORM_WIN32)
#	if defined(ENGINE_STATIC_LIB)
#		define		ENGINE_API
#	else
#		if defined(ENGINE_EXPORTS)
#			define	ENGINE_API		__declspec(dllexport)
#		else
#			define	ENGINE_API		__declspec(dllimport)
#		endif
#	endif
#else
#   if defined(ENGINE_STATIC_LIB)
#       define		ENGINE_API
#   else
#       if defined(ENGINE_EXPORTS)
#           define  ENGINE_API      __attribute__((visibility("default")))
#		else
#			define	ENGINE_API
#		endif
#	endif
#endif

#pragma warning(disable: 4251)

#endif // __ENGINE_H__