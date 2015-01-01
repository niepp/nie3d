#pragma once

#ifndef __CONFIG_H__
#define __CONFIG_H__

struct CORE_API Config
{
	Bool enable_debug_draw;
	static Config& Instance();
};

#endif // __CONFIG_H__