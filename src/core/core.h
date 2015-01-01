#pragma once

#ifndef __CORE_H__
#define __CORE_H__

#include "platform.h"

#if (PLATFORM == PLATFORM_WIN32)
#	if defined(CORE_STATIC_LIB)
#		define		CORE_API
#	else
#		if defined(CORE_EXPORTS)
#			define	CORE_API			__declspec(dllexport)
#		else
#			define	CORE_API			__declspec(dllimport)
#		endif
#	endif
#else
#   if defined(CORE_STATIC_LIB)
#       define		CORE_API
#   else
#       if defined(CORE_EXPORTS)
#           define  CORE_API            __attribute__((visibility("default")))
#		else
#			define	CORE_API
#		endif
#	endif
#endif

#include "types.h"
#include "log.h"
#include "math/mathdef.h"
#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"
#include "math/quaternion.h"
#include "math/matrix.h"
#include "math/rect.h"
#include "math/color.h"
#include "math/sphere.h"

#include "stringdef.h"
#include "refobject.h"
#include "variant.h"
#include "stringutils.h"
#include "filepath.h"
#include "fileutils.h"
#include "xmlutils.h"
#include "timer.h"
#include "config.h"

#include "renderer/irenderdef.h"
#include "renderer/renderstate.h"
#include "renderer/shadermacro.h"
#include "renderer/irenderer.h"

#endif //__TYPES_H__
